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
* VArduino.h - Header file of virtual Arduino source file.
*
*/

#define WIN32_LEAN_AND_MEAN

#ifndef __VARDUINO_H__
#define __VARDUINO_H__

#include <stdint.h>
#include <string>
#include <list>
#include <map>
#include <queue>

#define INPUT WININPUT
#include <windows.h>
#include <sysinfoapi.h>
#include <minwinbase.h>
#include <synchapi.h>
#undef INPUT

// Select microcontroller here
#define __AVR_ATmega328P__
// __AVR_ATmega168__ , __AVR_ATmega8__ , __AVR_ATmega1280__
// __AVR_ATmega2560__ , __SAM3X8E__

#define PI ((double) 3.141592654)

typedef uint8_t byte;

enum PinModes { INPUT, OUTPUT, INPUT_PULLUP };
enum OutputLevel { LOW, HIGH };
enum class InterruptMode { LOW, CHANGE, RISING, FALLING }; // This will cause issues in the future with sketches using interrupts.
enum Radix { DEC, OCT, HEX };

/* Arduino library functions */

extern bool allowinterrupts;
void noInterrupts(void);
void interrupts(void);
void attachInterrupt(int, void (*)(void), InterruptMode);
void detachInterrupt(int);

unsigned long millis(void);
unsigned long micros(void);

unsigned long micros(void);
void delay(unsigned long);
void delayMicroseconds(unsigned long);

void pinMode(uint8_t, PinModes);
void digitalWrite(uint8_t, OutputLevel);
OutputLevel digitalRead(uint8_t);
void analogWrite(uint8_t, uint16_t);
unsigned long pulseIn(uint8_t, OutputLevel);

const char* pgm_read_word(const char* const*);
void strcpy_P(char*, const char*);

class String
{
public:
	String();
	String(const char*);
	String(float, Radix = Radix::DEC);
	String(int, Radix=Radix::DEC);
	String(unsigned int, Radix = Radix::DEC);
	String(unsigned long, Radix = Radix::DEC);

	unsigned int length(void);
	void toCharArray(char*, unsigned int);
	const char* c_str(void);
	char charAt(unsigned int);
	String operator+(String);
	String operator+(const char*);
	std::string std_string(void);

private:
	std::string str;
};

/* Virtual Arduino specific things (not part of the actual Arduino library) */
namespace vard
{
	// Interrupts
	typedef struct interrupt
	{
		int number;
		void (*ISR)(void);
		InterruptMode mode;
	} Interrupt;

	extern std::list<Interrupt> intregistry;

	// Timing
	extern unsigned long sysbasetime;

	// Console
	enum class Level { FATAL, ERR, SEVERE, WARNING, INFO };
	void logevent(Level, const char*, ...);
	extern HANDLE winconsole;

	// Pins
	extern std::map<uint8_t, PinModes> pinregistry;

	class DigitalInputPin
	{
	public:
		DigitalInputPin(uint8_t, OutputLevel (*eventHandler)(void));

		OutputLevel read(void);
		uint8_t getNumber(void);

	protected:
		uint8_t pin;
		OutputLevel(*eventHandler)(void);
	};

	extern std::map<uint8_t, DigitalInputPin*> digital_input_pin_registry;
	void attach_digital_input_pin(DigitalInputPin*);
	void detach_digital_input_pin(DigitalInputPin*);

	// SD
	extern const char* sdvolume_root;

	// Serial
	class HardwareSerial
	{
	public:
		HardwareSerial(void);
		~HardwareSerial(void);

		bool begin(unsigned long);
		void flush(void);

		void print(const char*);
		void println(const char*);
		void print(String);
		void println(String);
		void print(int16_t, Radix = DEC);
		void println(int16_t, Radix = DEC);
		void write(uint8_t);
		void write(uint8_t*, unsigned int);
		uint8_t read(void);
		void readBytes(uint8_t*, unsigned int);
		String readString(void);

		unsigned int available(void);
		void setTimeout(unsigned long);

	private:
		bool isavailable = true, isconnected = false;
		unsigned int buffsize = 0, pos = 0;
		uint8_t* readbuffer = NULL;
	};

	// I2C device emulation
	class I2C_Device
	{
	public:
		I2C_Device(uint8_t, void (*eventHandler)(uint8_t*, unsigned int));

		uint8_t getAddress(void);
		void recieve(uint8_t*, unsigned int);
		std::queue<uint8_t>* transmit(unsigned int);
		void push(uint8_t);

	protected:
		uint8_t address;
		std::queue<uint8_t> writebuffer;
		void (*eventHandler)(uint8_t*, unsigned int);
	};

	extern std::map<uint8_t, I2C_Device*> i2c_device_registry;
	void attach_I2C_Device(I2C_Device*);
	void detach_I2C_Device(I2C_Device*);
}

extern vard::HardwareSerial Serial;

#endif