/*
* MIT License
*
* Copyright (c) 2020 Robert Hutter
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include <Slave.h>

/**
*	Constructor for the SCOM slave class.
*
*/
SRL::Slave::Slave()
{
	status = WAITING_FOR_SIGNAL;
	waitingForSignalFrom = 0L;
	messageId = 0;
	lastSentSignal = NULL;
	lastSentInfoMessage = NULL;
	ABF_attempts = 0;
}

/**
*	Destructor for the SCOM slave class.
*
*/
SRL::Slave::~Slave()
{
	delete lastSentSignal;
	delete lastRecievedSignal;
	delete lastSentInfoMessage;
	delete lastRecievedInfoMessage;
}

/**
*	Open the SCOM communication. Waits for the master to send SOH signal.
*
*	@param baudRate The serial connection's baud rate.
*/
uint8_t SRL::Slave::openSCOM(long baudRate)
{
	Serial.begin(baudRate);
	
	// Wait for SOH signal
	while(status != OK_CONTINUE && status != CONNECTION_CLOSED) {updateSCOM();}
	
	if (status == CONNECTION_CLOSED)
	{
		return false;
	}
	
	// Send protocol version
	sendSignal(VERSION);
	while(status != OK_CONTINUE && status != CONNECTION_CLOSED) {updateSCOM();}
	
	if (status == CONNECTION_CLOSED)
	{
		return false;
	}
	
	// Get responce
	status = WAITING_FOR_SIGNAL;
	waitingForSignalFrom = millis();
	while(status != OK_CONTINUE) {updateSCOM();}
	
	return lastRecievedSignal->getMessage() == ETX;
}

/**
*	Updates the SCOM communication by reading serial input.
*
*
*/
void SRL::Slave::updateSCOM(void)
{	
	// Wait for a complete message to arrive
	if (Serial.available() >= SIGNAL_LENGTH ||
		(status == WAITING_FOR_INFO_MESSAGE && Serial.available() > 0)
	)
	{
		Signal* recievedSignal;
		InfoMessage* recievedInfoMessage;
	
		if (status != WAITING_FOR_INFO_MESSAGE)
		{
			recievedSignal = readSignal();
			
 			if (recievedSignal->getMessageId() >= messageId)
 			{
 				messageId = recievedSignal->getMessageId() + 1;
 			}
			
			// Preprocess status
			switch (status)
			{
				case WAITING_FOR_REP_MESSAGE:
					status = OK_CONTINUE;
				break;
				
				case WAITING_FOR_SIGNAL:
					status = OK_CONTINUE;
				break;
				
				case WAITING_FOR_MORE_ANT:
					status = WAITING_FOR_ANT;
				break;
				
				default:
					break;
			}
		}
		else
		{
			recievedInfoMessage = readInfoMessage();
		}
	
		switch (status)
		{
			// Recieved a signal
			case OK_CONTINUE:
				switch (recievedSignal->getMessage())
				{
					case ANT:
						status = WAITING_FOR_MORE_ANT;
						antRecievedAt = millis();
					break;
				
					case ABF:
						sendSignal(ANT);
					break;
					
					default:
						delete lastRecievedSignal;
						lastRecievedSignal = new Signal(recievedSignal);
						delete lastSentSignal;
						lastSentSignal = NULL;
						sendSum(recievedSignal->getSum());
					break;
				}
			break;
			
			case WAITING_FOR_INFO_MESSAGE:
				sendSum(recievedInfoMessage->getSum());
				delete lastRecievedSignal;
				lastRecievedInfoMessage = new InfoMessage(recievedInfoMessage);
				delete lastSentInfoMessage;
				lastSentInfoMessage = NULL;
			break;
			
			// ACK, ABF or resent message has arrived
			case WAITING_FOR_ACK:
				switch (recievedSignal->getMessage())
				{
					case ACK:
						status = OK_CONTINUE;
						
						/*if (lastRecievedSignal->getMessage() == STX)
						{
							status = WAITING_FOR_INFO_MESSAGE;
						}
						
						if (lastRecievedInfoMessage != NULL)
						{
							// Do something with confirmed infoMessage
						}*/
					break;
					
					case ABF:
						sendSignal(ANT);
					break;
					
					default:
						delete lastRecievedSignal;
						lastRecievedSignal = new Signal(recievedSignal);
						sendSum(recievedSignal->getMessage());
					break;
				}
			break;
			
			// Recieved responce
			case WAITING_FOR_ASCII_SUM:
				if (recievedSignal->getMessage() == awaitedSum)
				{
					sendSignal(ACK);
				}
				else
				{
					sendSignal(lastSentSignal->getMessage());
				}
			break;
			
			// ABF has arrived
			case IN_ACK_TIMEOUT_BUFFER:
				if (recievedSignal->getMessage() == ABF)
				{
					// Deal with timeout
					sendSignal(ANT);
				}
			break;
			
			// ANT or ABF has arrived
			case WAITING_FOR_ANT:
				switch(recievedSignal->getMessage())
				{
					case ANT:
						status = WAITING_FOR_MORE_ANT;
						antRecievedAt = millis();
					break;
					
					case ABF:
						sendSignal(ANT);
					break;
					
					default:
						sendSignal(ABF);
					break;
				}
			break;
			
			case WAITING_FOR_SIGNAL:
				if (waitingForSignalFrom != 0L)
				{
					if (millis() >= waitingForSignalFrom + SIGNAL_TIMEOUT)
					{
						status = CONNECTION_CLOSED;
					}
				}
			break;
			
			default:
			break;
		}
		
		delete recievedSignal;
		delete recievedInfoMessage;
	}
	else // New message did not arrive
	{
		switch(status)
		{
			case WAITING_FOR_ASCII_SUM:
				if (millis() >= messageSentAt + TIMEOUT)
				{
					// If sum didnt arrive
					sendSignal(ABF);
				}
			break;
		
			case WAITING_FOR_ACK:
				if (millis() >= sumSentAt + ACK_TIMEOUT)
				{
					// ACK didnt arrive, send ABF
					sendSignal(ABF);
				}
			break;
			
			case IN_ACK_TIMEOUT_BUFFER:
				if (millis() >= ackSentAt + ACK_TIMEOUT)
				{
					status = OK_CONTINUE;
					delete lastSentSignal;
					lastSentSignal = NULL;
				}
			break;
			
			case WAITING_FOR_ANT:
				if(millis() >= abfSentAt + ABF_INTERVAL)
				{
					sendSignal(ABF);
				}
			break;
			
			case WAITING_FOR_MORE_ANT:
				if(millis() >= antRecievedAt + REFRESH_INTERVAL)
				{
					if (lastSentSignal == NULL)
					{
						status = WAITING_FOR_REP_MESSAGE;
					}
					else
					{
						sendSignal(lastSentSignal->getMessage());
					}
				}
			break;
			
			default:
			
			break;
		}
	}
}

/**
*	Sends a message over Serial.
*
*	@param message
*/
void SRL::Slave::sendSignal(int16_t message)
{
	switch (message)
	{
		case ACK:
			status = IN_ACK_TIMEOUT_BUFFER;
			ackSentAt = millis();
			writeSignal(&Signal(messageId++, message));
		break;
		
		case ABF:
			if (ABF_attempts++ >= MAX_ABF_ATTEMPTS)
			{
				status = CONNECTION_CLOSED;
			}
			else
			{
				status = WAITING_FOR_ANT;
				abfSentAt = millis();
				writeSignal(&Signal(messageId++, message));
			}
		break;
		
		case ANT:
			if(lastSentSignal == NULL)
			{
				status = WAITING_FOR_REP_MESSAGE;
			}
			else
			{
				status = WAITING_FOR_MORE_ANT;
				/*antSentAt*/antRecievedAt = millis();
			}
			
			writeSignal(&Signal(messageId++, message));
		break;
		
		default:
			delete lastSentSignal;
			lastSentSignal = new Signal(messageId++, message);
			status = WAITING_FOR_ASCII_SUM;
			awaitedSum = lastSentSignal->getSum();
			messageSentAt = lastSentSignal->getCreatedAt();
			writeSignal(lastSentSignal);
		break;
	}
}

/**
*	Sends an ASCII sum over Serial.
*
*	@param sum
*/
void SRL::Slave::sendSum(int16_t sum)
{
	status = WAITING_FOR_ACK;
	sumSentAt = millis();
	writeSignal(&Signal(messageId++, sum));
}
