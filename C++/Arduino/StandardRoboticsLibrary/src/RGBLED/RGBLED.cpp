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
*
* RGBLED.cpp - Source code of class and functions regarding RGB capable light
*   emitting diodes.
*
*/

#include <RGBLED.h>

/**
*	Compares two SRL::color data types to see if they match.
*
*	@param a
*	@param b
*/
bool SRL::compareColor(color a, color b)
{
	return (a.red == b.red && a.green == b.green && a.blue == b.blue);
}

/**
*	Constructor for the class RGB LED.
*
*	@param redpin
*	@param greenpin
*	@param bluepin
*/
SRL::rgbled::rgbled(uint8_t redpin, uint8_t greenpin, uint8_t bluepin) : Component(RGBLED_COMPONENT_NAME, Component::LIGHT)
{
	*this->redpin = redpin;
	*this->greenpin = greenpin;
	*this->bluepin = bluepin;
	
	pinMode(redpin, OUTPUT);
	pinMode(greenpin, OUTPUT);
	pinMode(bluepin, OUTPUT);
	
	setColor(WHITE);
	setState(OFF);
}

/**
*	Destructor for the class RGB LED.
*
*/
SRL::rgbled::~rgbled()
{
	delete ledcol;
	delete redpin;
	delete greenpin;
	delete bluepin;
}

/**
*	Set the color of the RGB LED.
*
*	@param red
*	@param green
*	@param blue
*
*/
void SRL::rgbled::setColor(byte red, byte green, byte blue)
{
	setColor(color {red, green, blue});
}

/**
*	Returns the current color of the RGB led.
*
*/
SRL::color SRL::rgbled::getColor(void)
{
	return *this->ledcol;
}

/**
*	Sets the color of the RGB LED.
*
*	@param ledcol
*/
void SRL::rgbled::setColor(color ledcol)
{
	(*this->ledcol).red = ledcol.red;
	(*this->ledcol).green = ledcol.green;
	(*this->ledcol).blue = ledcol.blue;
	
	analogWrite(*redpin, ledcol.red);
	analogWrite(*greenpin, ledcol.green);
	analogWrite(*bluepin, ledcol.blue);
}

/**
*	Turns the LED off.
*
*/
void SRL::rgbled::turnOff(void)
{
	*ledstate = OFF;
	digitalWrite(*redpin, LOW);
	digitalWrite(*greenpin, LOW);
	digitalWrite(*bluepin, LOW);
}

/**
*	Turns the led on.
*
*/
void SRL::rgbled::turnOn(void)
{
	*ledstate = ON;
	setColor(*ledcol);
}

/**
*	Returns the state of the led. True = ON, false = OFF.
*
*/
bool SRL::rgbled::getState(void)
{
	return *ledstate;
}

/**
*	Sets the state of the led. True = ON, false = OFF.
*
*/
void SRL::rgbled::setState(bool state)
{
	*ledstate = state;
	if (state)
		turnOn();
	else
		turnOff();
}
