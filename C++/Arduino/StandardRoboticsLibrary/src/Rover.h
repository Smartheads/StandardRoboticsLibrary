// Rover.h

#ifndef _ROVER_h
#define _ROVER_h

#include <SRL.h>
#include <NewPing.h>
#include <MPU6050_tockn.h>
#include <DueTimer.h>
#include <Encoder.h>
#include <Angle.h>
#include <Vector.h>
#include <Motor.h>

namespace SRL
{
	class Rover
	{
		public:
			Rover(SRL::Motor* leftMotor, SRL::Motor* rightMotor, double x = 0.0, double y = 0.0, float direction = 0.0f);
			Rover(SRL::Motor* leftMotor, SRL::Motor* rightMotor, NewPing* leftSonar, NewPing* rightSonar, double x = 0.0, double y = 0.0, float direction = 0.0f);
			Rover(SRL::Motor* leftMotor, SRL::Motor* rightMotor, NewPing* leftSonar, NewPing* rightSonar, MPU6050* mpu, double x = 0.0, double y = 0.0, float direction = 0.0f);
			Rover(SRL::Motor * leftMotor, SRL::Motor * rightMotor, NewPing * leftSonar, NewPing * rightSonar, Encoder* leftEncoder, Encoder* rightEncoder, MPU6050* mpu, double x = 0.0, double y = 0.0, float direction = 0.0f);

			void init(void);

			/* Movement commands */
			void forward(float distance = 35.0f);
			void backwards(float distance = 35.0f);
			void turnRight(float amount = 90.0f);
			void turnLeft(float amount = 90.0f);

			/* Getters & setters */
			float getDirection(void);
			double getX(void);
			double getY(void);

			/* Other */
			void calcGyroOffsets(bool console = false, unsigned int sampleSize = 3000);
			
			/* Enums */
			enum consts
			{
				GYROSCOPE_INTERVAL = 100,
				UPDATE_POSITION_INTERAL = 500
			};

		private:
			SRL::Motor* leftMotor;
			SRL::Motor* rightMotor;
			Encoder* leftEncoder;
			Encoder* rightEncoder;
			NewPing* leftSonar;
			NewPing* rightSonar;
			MPU6050* mpu;

			double x;
			double y;
			SRL::Angle direction;

			void updateDirection(void);
			void updatePosition(void);
	};
}
#endif