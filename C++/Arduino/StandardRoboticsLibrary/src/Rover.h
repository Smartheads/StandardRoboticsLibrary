/*
* MIT License
*
* Copyright (c) 2018 Robert Hutter
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
#ifndef _ROVER_h
#define _ROVER_h

#include <SRL.h>
#include <Angle.h>
#include <Vector.h>
#include <Tank.h>
#include <Component.h>
#include <stdarg.h>
#include <Encoder.h>
#include <AccelGyro.h>
#include <vector>
#include <Statistics.h>

namespace SRL
{
	class Rover : protected Tank
	{
		public:
			/* Constructors */
			Rover(SRL::Motor* leftMotor, SRL::Motor* rightMotor,
				 double x = 0.0, double y = 0.0, float direction = 0.0f);

			/* Destructor */
			~Rover(void);

			/* Initializer */
			void initialize(void);

			/* Movement commands */
			void forward(double distance = 35.0f);
			void backward(double distance = 35.0f);
			void turnRight(double amount = 90.0f);
			void turnLeft(double amount = 90.0f);
			void goTo(double x, double y);
			void turnTo(SRL::Angle dir);
			void stop(void);

			/* Timer methods */
			void correctMotors(void);
			void updatePosition(void);

			/* Getters & setters */
			float getDirection(void);
			void setDirection(float direction);
			double getX(void);
			double getY(void);
			void setX(double x);
			void setY(double y);

			void setAccelGyro(AccelGyro* accelGyro);
			void setLeftEncoder(Encoder* leftEncoder);
			void setRightEncoder(Encoder* rightEncoder);

			AccelGyro* getAccelGyro(void);
			Encoder* getLeftEncoder(void);
			Encoder* getRightEncoder(void);
			Motor* getRightMotor(void);
			Motor* getLeftMotor(void);

			/* Enums */
			enum Intervals
			{
				UPDATE_POSITION_INTERVAL = 235000,
				CORRECT_MOTORS_INTERVAL = 250
			};

		protected:
			/* Position related fields */
			double x, y;
			SRL::Angle direction;

			/* Component related fields */
			SRL::Encoder* leftEncoder;
			SRL::Encoder* rightEncoder;
			SRL::AccelGyro* accelGyro;

			/* Movement related fields */
			bool turning = false;
			SRL::Angle turnGoal;

			bool movingStraight = false;
			double xGoal, yGoal;

	 private:
			/* Movement related methods */
			float getCorrectionSpeed(float x);
	};
}
#endif
