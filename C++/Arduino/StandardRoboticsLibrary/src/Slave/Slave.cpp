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
	status = OK_CONTINUE;
	messageId = 0;
}

/**
*	Destructor for the SCOM slave class.
*
*/
SRL::Slave::~Slave()
{

}

/**
*	Open the SCOM communication. Waits for the master.
*
*	@param baudRate The serial connection's baud rate.
*/
uint8_t SRL::Slave::openSCOM(unsigned int baudRate)
{
	Serial.begin(baudRate);
	
	Serial.println("Hello world: " + String(Signal::length, DEC)); 
	
	// Wait for SOH signal
	while(Serial.available() < Signal::length) {Serial.println(Serial.available());delay(100);}
	do {updateSCOM();} while(status != OK_CONTINUE);
	
	return true;
}

/**
*	Updates the SCOM communication by reading serial input.
*
*
*/
void SRL::Slave::updateSCOM(void)
{	
	// Wait for a complete message to arrive
	if (Serial.available() >= Signal::length ||
		(status == WAITING_FOR_INFO_MESSAGE && Serial.available() > 0)
	)
	{
		Signal* recievedSignal;
	
		if (status != WAITING_FOR_INFO_MESSAGE)
		{
			recievedSignal = readSignal();
		}
	
		switch (status)
		{
			case OK_CONTINUE: // Recieved a signal
				sendSum(recievedSignal->getSum());
			break;
			
			case WAITING_FOR_ACK: // Recieved ACK
			break;
			
			default:
			break;
		}
		
		delete recievedSignal;
	}
	else
	{
		switch(status)
		{
			case WAITING_FOR_ACK:
				if (millis() >= sumSentAt + ACK_TIMEOUT)
				{
					// ACK didnt arrive, send ABF
					sendSignal(ABF);
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
			
		break;
		
		case ABF:
			
		break;
		
		default:
			delete lastSentSignal;
			lastSentSignal = new Signal(messageId++, message);
			writeSignal(lastSentSignal);
			status = WAITING_FOR_ASCII_SUM;
			awaitedSum = lastSentSignal->getSum();
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
	writeSignal(&Signal(messageId++, sum));
	status = WAITING_FOR_ACK;
	sumSentAt = millis();
}
