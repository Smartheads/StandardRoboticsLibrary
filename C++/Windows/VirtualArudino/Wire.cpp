/*
* MIT License
*
* Copyright (c) 2021 Robert Hutter
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
*
* Wire.cpp - Source code of Wire for Virtual Arduino runtime environment.
*
*/

#include "Wire.h"

vard::HardwareWire Wire;

/**
* Contructor for the class vard::HardwareWire.
*
*/
vard::HardwareWire::HardwareWire(void)
{

}

/**
* Destructor for class vard::HardwareWire.
*
*/
vard::HardwareWire::~HardwareWire(void)
{
	
}

/**
* Begins a Wire connection. Returns 0 if fails.
*
* @return True if successful.
*/
bool vard::HardwareWire::begin(void)
{
	if (this->isavailable)
	{
		this->isconnected = true;
		vard::logevent(Level::INFO, "Wire.begin called. Successful connection.");
		return true;
	}

	vard::logevent(Level::ERR, "Wire.begin called. Connection failed.");
	return false;
}

/**
* Prepares for a transmission to the given address.
*
* @param addr Address to send data to.
*/
bool vard::HardwareWire::beginTransmission(uint8_t addr)
{
	if (this->isconnected)
	{
		// Check if device is in registry
		if (i2c_device_registry.find(addr) != i2c_device_registry.end())
		{
			targetaddr = addr;
			writebuffer = std::queue<uint8_t>();

			vard::logevent(Level::INFO, "Wire.beginTransmission called. addr=%#x", addr);
			
			return true;
		}
		
		this->targetaddr = 0;
		vard::logevent(Level::ERR, "Wire.beginTransmission called. Device not found. addr=%#x", addr);
		
		return false;
	}

	vard::logevent(Level::ERR, "Wire.beginTransmision called. Connection not open.");
	return false;
}

/**
* Writes a byte through Wire.
* Requires an open connection.
* 
* @param val Byte to send
*/
bool vard::HardwareWire::write(uint8_t val)
{
	if (targetaddr)
	{
		writebuffer.push(val);

		vard::logevent(Level::INFO, "Wire.write called. addr=%#x value=%u", targetaddr, val);
		return true;
	}

	vard::logevent(Level::ERR, "Wire.write called. No target address.");
	return false;
}

/**
* Writes a byte buffer over Wire.
* 
* @param buff Pointer to byte buffer.
* @param size Size of buffer.
* @return True if successful.
*/
bool vard::HardwareWire::write(uint8_t* buff, uint8_t size)
{
	if (targetaddr)
	{
		for (int i = 0; i < size; i++)
		{
			this->write(*(buff + i));
		}

		return true;
	}

	vard::logevent(Level::ERR, "Wire.write called. No target address.");
	return false;
}

/**
* Closes a transmission sequence.
*
* @return True if transmission successful.
*/
bool vard::HardwareWire::endTransmission(void)
{
	if (targetaddr)
	{
		unsigned int size = writebuffer.size();
		uint8_t* buff = new uint8_t[size];
		for (int i = 0; i < size; i++)
		{
			buff[i] = writebuffer.front();
			writebuffer.pop();
		}

		// Transmit byte buffer to i2c device
		i2c_device_registry.at(targetaddr)->recieve(buff, size);
		targetaddr = 0;

		logevent(Level::INFO, "Wire.endTransmission called.");
		return true;
	}

	logevent(Level::ERR, "Wire.endTransmission called. No target address.");
	this->targetaddr = 0;
	return false;
}

/**
* Opens a transmission sequence to read.
* 
* @param addr Address to read from.
* @param size Number of bytes to read into buffer.
* @return Length of bytes recieved from device.
*/
uint8_t vard::HardwareWire::requestFrom(uint8_t addr, uint8_t size)
{
	if (this->isconnected)
	{
		// Check to see if device is registered
		if (i2c_device_registry.find(addr) != i2c_device_registry.end())
		{
			std::queue<uint8_t>* buffer = i2c_device_registry.at(addr)->transmit(size);
			readbuffer = std::queue<uint8_t>();
			std::swap(*buffer, readbuffer);
			delete buffer;

			vard::logevent(Level::INFO, "Wire.requestFrom called. addr=%#x size=%u recieved=%u", addr, size, (int)readbuffer.size());
			return readbuffer.size();
		}

		vard::logevent(Level::ERR, "Wire.requestFrom called. No target address.");
		return 0;
	}

	vard::logevent(Level::ERR, "Wire.requestFrom called. Connection not open.");
	return 0;
}

/**
* Read a byte from the read buffer.
* 
* @return A byte from the buffer.
*/
uint8_t vard::HardwareWire::read(void)
{
	uint8_t result = readbuffer.front();
	readbuffer.pop();
	return result;
}
