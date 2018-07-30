# Standard Robotics Library (SCSRL)

About
-
The Standard Robotics Library project is a project aimed to create a universal framework for robotics development on desktop and mobile (Arduino, Raspberry PI) platforms.

Planed packages
-
Desktop
* Communications package
* Control package
* Math package
* Mapping package
* Data analysis

Mobile
* Communications package
* Sensor package
* Control package

Usage
-
Using the SRL is very simple.
1. Download the SRL Arduino library (C++/Arduino/StandardRoboticsLibary) into your arduino library folder.
2. Include the header file of the desired package. (i.e #include <Vector.h>)
3. Done. See the wiki for more information on how to use the different packages.
Example:
```
// MPU6050 Example
#include <MPU6050.h>
using namespace SRL;

MPU6050* mpu;

void setup()
{
        Serial.begin(115200);
        mpu = new MPU6050(0x68);
        mpu->initialize();
}

void loop()
{
       Serial.print("x: "); Serial.print(mpu->getAccelX());
       Serial.print(" y: "); Serial.print(mpu->getAccelY());
       Serial.print(" z: "); Serial.println(mpu->getAccelZ());
}
```

How to contribute
-
You can't. :( This project is only public, because we are too cheap to buy Git-Hub premium. For more information please contact: rohu747@gmail.com
