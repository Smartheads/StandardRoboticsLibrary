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
*/
#include "Sonar.h"

/**
* Class sonar's constructor.
*
* @param triggerPin Sonar's trigger pin.
* @param echoPin Sonar's echo pin.
* @param name The sonar's name.
* @param maxDistanceCm The furthest the sensor will detect objects (in cm).
*/
SRL::Sonar::Sonar(uint8_t triggerPin, uint8_t echoPin, unsigned long maxDistanceCm)
{
  setTriggerPin(triggerPin);
  setEchoPin(echoPin);
  setMaxDistance(maxDistanceCm);
}

/**
* Retuns the distance of any objects in front of the sensor.
*
* @return
* Returns the distance in cm.
*/
double SRL::Sonar::pingCm(void)
{
  return convertCm(ping());
}

/**
* Retuns the distance of any objects in front of the sensor.
*
* @return
* Returns the distance in mm.
*/
double SRL::Sonar::pingMm(void)
{
  return pingCm() * 10;
}

/**
* Converts the argument into distance.
*
* @param us The time it takes to ping an object in micro seconds.
* @return
* Returns the distance of the object in mm.
*/
double SRL::Sonar::convertMm(unsigned long us)
{
  return convertCm(us) * 10;
}

/**
* Pings the objects in front of the sensor and returns the reading's median.
* It will discard any really off readings.
* Note: untested since last change.
*
* @param interations The number of measurments to take.
* @return
* Returns the median of the pings in micro seconds. Default value: 5
*/
unsigned long SRL::Sonar::pingMedian(unsigned int interations)
{
  unsigned long last, t;
  unsigned long* samples = new unsigned long[interations];
  unsigned int j, i = 0;
  samples[0] = NO_ECHO;

  while (i < interations)
  {
    t = micros();
    last = ping();

    if (last != NO_ECHO)
    {
      if (i > 0)  // Dont sort the first sample
      {
        for (j = i; j > 0 && samples[j - 1] < last; j--) // Sort using the insersion method
        {
          samples[j] = samples[j - 1];
        }
      }
      else
      {
        j = 0;
      }

      samples[j] = last;
      i++;
    }
    else
    {
      interations--;
    }

    if (i < interations && micros() - t < PING_MEDIAN_DELAY)
    {
      delay((PING_MEDIAN_DELAY + t - micros()) / 1000); // Delay between readings
    }
  }

  unsigned long result = samples[interations >> 1];
  delete[] samples;
  return result;
}

/**
* Pings the objects in front of the sensor and returns the reading's median.
* It will discard any really off readings.
*
* @param interations The number of measurments to take.
* @return
* Returns the median of the pings in cm. Default value: 5
*/
double SRL::Sonar::pingMedianCm(unsigned int interations)
{
  return convertCm(pingMedian(interations));
}

/**
* Pings the objects in front of the sensor and returns the reading's median.
* It will discard any really off readings.
*
* @param interations The number of measurments to take.
* @return
* Returns the median of the pings in mm. Default value: 5
*/
double SRL::Sonar::pingMedianMm(unsigned int interations)
{
  return pingMedianCm(interations) * 10;
}

uint8_t SRL::Sonar::getTriggerPin(void)
{
  return triggerPin;
}

void SRL::Sonar::setTriggerPin(uint8_t triggerPin)
{
  this->triggerPin = triggerPin;
  pinMode(this->triggerPin, OUTPUT);
}

uint8_t SRL::Sonar::getEchoPin(void)
{
  return echoPin;
}

void SRL::Sonar::setEchoPin(uint8_t echoPin)
{
  this->echoPin = echoPin;
  pinMode(this->echoPin, INPUT);
}

double SRL::Sonar::getMaxDistance(void)
{
  return convertCm(maxDistance);
}

void SRL::Sonar::setMaxDistance(unsigned long us)
{
  this->maxDistance = us;
}

void SRL::Sonar::setMaxDistance(double cm)
{
  this->maxDistance = convertUs(cm);
}
