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
* Servo.cpp - Source code for virtual Servo. Part of the Virtual Arduino runtime
*	enviroment.
*
*/

#include "Servo.h"

/**
* Constructor method for class Servo.
*
*/
Servo::Servo(void)
{

}

/**
* Destructor method for class Servo.
*
*/
Servo::~Servo(void)
{

}

/**
*   Attaches a pin to virtual servo configuration. 
* 
*   @param pin PWM pin attached to servo.
*/
uint8_t Servo::attach(unsigned int pin)
{
    vard::logevent(vard::Level::INFO, "Servo.attach called. pin=%u", pin);
    return true;
}

/**
*   Moves servo armature to specified position. 
* 
* @param pos Position to move servo armature to.
*/
void Servo::write(int pos)
{
    vard::logevent(vard::Level::INFO, "Servo.write called. pos=%d", pos);
}
