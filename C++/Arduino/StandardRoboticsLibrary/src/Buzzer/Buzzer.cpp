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
*
* Buzzer.cpp - Source code of class and functions regarding piezzo buzzers.
*
*/

#include <Buzzer.h>

/**
*	Constructor for the class Buzzer.
*
*	@param pin
*/
Buzzer::Buzzer(uint8_t pin)
{
	*this->pin = pin;
	pinMode(pin, OUTPUT);
	
	setState(OFF);
}

/**
*	Destructor for the class Buzzer.
*
*/
Buzzer::~Buzzer()
{
	delete pin;
	delete state;
}

/**
*	Turns the buzzer off.
*
*/
void Buzzer::turnOff(void)
{
	*state = OFF;
	digitalWrite(*pin, LOW);
}

/**
*	Turns the buzzer on.
*
*/
void Buzzer::turnOn(void)
{
	*state = ON;
	digitalWrite(*pin, HIGH);
}

/**
*	Sets the state of the buzzer. ON = true, OFF = false.
*
*/
void Buzzer::setState(bool state)
{
	if (state)
		turnOn();
	else
		turnOff();
}

/**
*	Returns the state of the buzzer. True = ON, false = OFF.
*
*/
bool Buzzer::getState(void)
{
	return *state;
}
