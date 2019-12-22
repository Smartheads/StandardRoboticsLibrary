/*
* MIT License
*
* Copyright (c) 2019 Robert Hutter
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
	Serial.setTimeout(2000);
	mode = SIGNAL_MODE;
	status = WAITING_FOR_MASTER_SIGNAL;
	
	// Wait for SOH signal
	waitForUpdate();
	
	// Get version number
	status = WAITING_FOR_MASTER_SIGNAL;
	waitForUpdate();
	
	// Send version number
	sendInt16(VERSION);
	awaitedSum = calculateSum(VERSION);
	
	// Wait for responce
	status = WAITING_FOR_MASTER_RESPONCE;
	waitForUpdate();
	
	// Get version compatibility information
	status = WAITING_FOR_MASTER_SIGNAL;
	waitForUpdate();
	
	status = WAITING_FOR_MASTER_SIGNAL;
	
	return lastMasterSignal == ETB;
}

/**
*	Waits for an update from the master.
*
*/
void SRL::Slave::waitForUpdate(void)
{
	while(status != IN_TIMEOUT_BUFFER)
	{
		updateSCOM();
		
		if (status == OK_CONTINUE)
			return;
	}
	
	for (int i = 0; i<10; i++)
	{
		delay(200);
		updateSCOM();
		
		if (status == OK_CONTINUE)
			return;
	}

	// Timeout error!
	exit(1);
}

/**
*	Updates the SCOM communication by reading serial input.
*
*
*/
void SRL::Slave::updateSCOM(void)
{
	//watchTimeout();	
	
	// Wait for a complete message to arrive
	if (Serial.available() > 1 || (status == WAITING_FOR_MASTER_COMMAND && Serial.available() > 0))
	{
		if (status == WAITING_FOR_MASTER_SIGNAL)
		{
			// Read an int16_t
			lastMasterSignal = readInt16();
			
			status = WAITING_FOR_MASTER_RESPONCE;
			
			sendInt16(calculateSum(lastMasterSignal));
		}
		else if (status == WAITING_FOR_MASTER_RESPONCE)
		{
			int16_t masterResponce = readInt16();
			
			if (masterResponce == ACK)
			{
				status = OK_CONTINUE;
				
				// Prepair for infomessage
				if (lastMasterSignal == STX)
				{
					status = WAITING_FOR_MASTER_COMMAND;					
				}
				
				if (mode == COMMAND_MODE)
				{
					status = WAITING_FOR_MASTER_SIGNAL;
					mode = SIGNAL_MODE;
				}

				return;
			}
			
			if (masterResponce == awaitedSum)
			{
				sendInt16(ACK);
				status = IN_TIMEOUT_BUFFER;
				return;
			}
			else
			{
				// TODO: Implement
				sendInt16(VERSION);
			}
		}
		else if (status == WAITING_FOR_MASTER_COMMAND)
		{
			String command = Serial.readString();
			
			status = WAITING_FOR_MASTER_RESPONCE;
			mode = COMMAND_MODE;
			
			sendInt16(calculateSum(command));
		}
		else if (status == IN_TIMEOUT_BUFFER)
		{
			int16_t masterResponce = readInt16();
			
			if (masterResponce == awaitedSum)
			{
				sendInt16(ACK);
				status = IN_TIMEOUT_BUFFER;
				return;
			}
		}
	}
	else if (status == IN_TIMEOUT_BUFFER)
	{
		status = OK_CONTINUE;
	}
	
}

/**
*	Watches communication timeouts.
*
*
*
void SRL::Slave::watchTimeout(void)
{

}*/
