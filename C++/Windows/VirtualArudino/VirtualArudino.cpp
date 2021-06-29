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
* VArduino.cpp - Source code for Virtual Arduino.
*
*/

#include "VirtualArduino.h"

std::list<vard::Interrupt> intregistry;
std::map<uint8_t, vard::I2C_Device*> vard::i2c_device_registry;
std::map<uint8_t, PinModes> vard::pinregistry;
std::map<uint8_t, vard::DigitalInputPin*> vard::digital_input_pin_registry;
vard::HardwareSerial Serial;
unsigned long vard::sysbasetime;
HANDLE vard::winconsole;
const char* vard::sdvolume_root;
bool allowinterrupts = true;

/**
* Disable interrupts. 
* 
*/
void noInterrupts(void)
{
	allowinterrupts = false;
}

/**
* Allow interrupts.
*
*/
void interrupts(void)
{
	allowinterrupts = true;
}

/**
* Attach a pin with specific mode to interrupt ISR
*	routine.
* 
* @param interrupt Number of interrupt
* @param ISR Pointer to function to execute on interrupt
* @param mode Interrupt mode, LOW, CHANGE, RISING, FALLING.
*/
void attachInterrupt(int interrupt, void (*ISR)(void), InterruptMode mode)
{
	vard::Interrupt i = vard::Interrupt();
	i.number = interrupt;
	i.ISR = ISR;
	i.mode = mode;
	intregistry.push_back(i);
}

/**
* Detach interrupt. Will effectively disable
*  the interrupt.
*
* @param interrupt Number of interrupt to detach.
*/
void detachInterrupt(int interrupt)
{
	for (std::list<vard::Interrupt>::iterator i = intregistry.begin(); i != intregistry.end(); ++i)
	{
		if ((*i).number == interrupt)
		{
			intregistry.erase(i);
			break;
		}
	}
}

/*
* Logs an event to the standard output.
*
* @param level Level of event.
* @param msg c String of message.
* @param enum level
*/
void vard::logevent(Level level, const char* format, ...)
{
	// Time
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	switch (level)
	{
	case vard::Level::FATAL:
		SetConsoleTextAttribute(winconsole, (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED));
		printf("[FATAL]");
		break;

	case vard::Level::ERR:
		SetConsoleTextAttribute(winconsole, (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED));
		printf("[ERROR]");
		break;

	case vard::Level::SEVERE:
		SetConsoleTextAttribute(winconsole, (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED));
		printf("[SEVERE]");
		break;

	case vard::Level::WARNING:
		SetConsoleTextAttribute(winconsole, (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE));
		printf("[WARNING]");
		break;

	default:
		SetConsoleTextAttribute(winconsole, (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_GREEN));
		printf("[INFO]");
		break;
	}
	SetConsoleTextAttribute(winconsole, (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY));
	printf("[%02d:%02d:%02d.%02d] ", systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
	SetConsoleTextAttribute(winconsole, (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN));

	va_list args;
	int len;
	char* buffer;

	va_start(args, format);
	len = _vscprintf(format, args) + 1; // _vscprintf doesn't count terminating '\0'
	buffer = (char*)malloc(len * sizeof(char));
	if (NULL != buffer)
	{
		vsprintf_s(buffer, len, format, args);
		puts(buffer);
		free(buffer);
	}
	else
	{
		puts(format);
	}
	va_end(args);
}

/**
* Returns Virtual Arduino system uptime in milliseconds.
*
* @return
*/
unsigned long millis(void)
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	unsigned long current = systime.wHour * 3600000 + systime.wMinute * 60000 + systime.wSecond * 1000 + systime.wMilliseconds;
	return current - vard::sysbasetime;
}

/**
* Returns Virtual Arduino system uptime in microseconds.
*
* @return
*/
unsigned long micros(void)
{
	return millis() * 1000;
}

/**
* Halts execution for a specified duration.
*
* @param milis Amount of time to halt in ms.
*/
void delay(unsigned long milis)
{
	Sleep(milis);
}

/**
* Halts execution for a specified duration.
* 
* @param micros Amount if time to halt in microseconds.
*/
void delayMicroseconds(unsigned long micros)
{
	if (micros > 1000) // Smaller then one millisecond
	{
		Sleep((DWORD)micros / 1000);
	}
	// else just skip, we cant sleep for less then a millisecond
}

/**
* Setup an GPIO pin for usage.
*
* @param pin Number of pin to configure.
* @param mode Configuration mode.
*/
void pinMode(uint8_t pin, PinModes mode)
{
	// Register pin
	std::map<uint8_t, PinModes>::iterator it = vard::pinregistry.find(pin);
	if (it == vard::pinregistry.end())
	{
		vard::pinregistry.emplace(pin, mode);
	}
	else
	{
		vard::pinregistry.at(pin) = mode;
	}

	switch (mode)
	{
	case INPUT:
		vard::logevent(vard::Level::INFO, "Pin %u set for INPUT.", pin);
		break;

	case OUTPUT:
		vard::logevent(vard::Level::INFO, "Pin %u set for OUTPUT.", pin);
		break;

	case INPUT_PULLUP:
		vard::logevent(vard::Level::INFO, "Pin %u set for INPUT_PULLUP.", pin);
		break;
	}
}

/**
* Toggles a GPIO pin as specified.
*
* @param uint8_t Pin to toggle
* @param uint8_t Logical level of state.
*/
void digitalWrite(uint8_t pin, OutputLevel level)
{
	// Check registry for pin
	std::map<uint8_t, PinModes>::iterator it = vard::pinregistry.find(pin);
	if (it == vard::pinregistry.end())
	{
		vard::logevent(vard::Level::ERR, "digitalWrite called. Pin not registered. pin=%u", pin);
	}
	else if (vard::pinregistry.at(pin) == PinModes::OUTPUT)
	{
		switch (level)
		{
		case LOW:
			vard::logevent(vard::Level::INFO, "digitalWrite called. pin=%u value=LOW", pin);
			break;

		case HIGH:
			vard::logevent(vard::Level::INFO, "digitalWrite called. pin=%u value=HIGH", pin);
			break;

		default:
			vard::logevent(vard::Level::ERR, "digitalWrite called. pin=%u value=undefined", pin);
			break;
		}
	}
	else
	{
		vard::logevent(vard::Level::ERR, "digitalWrite called. Pin not set for OUTPUT. pin=%u", pin);
	}
}

/**
* Read the digital state of a pin.
*
* @param pin Pin to read.
*/
OutputLevel digitalRead(uint8_t pin)
{
	if (vard::pinregistry.find(pin) == vard::pinregistry.end())
	{
		vard::logevent(vard::Level::ERR, "digitalRead called. Pin not registered. pin=%u", pin);
	}
	else if (vard::pinregistry.at(pin) == PinModes::INPUT || vard::pinregistry.at(pin) == PinModes::INPUT_PULLUP)
	{
		// Check to see if pin is attached (not required)
		if (vard::digital_input_pin_registry.find(pin) != vard::digital_input_pin_registry.end())
		{
			OutputLevel result = vard::digital_input_pin_registry.at(pin)->read();
			vard::logevent(vard::Level::INFO, "digitalRead called. pin=%u val=%u", pin, result);
			return result;
		}

		vard::logevent(vard::Level::INFO, "digitalRead called. No virtual pin connected. pin=%u", pin);

		return OutputLevel::LOW; // Always return LOW if no virtual pin connected
	}
	else
	{
		vard::logevent(vard::Level::ERR, "digitalRead called. Pin not set for INPUT. pin=%u", pin);
	}
	return OutputLevel::LOW;
}

/**
* Outputs an analog PWM signal on a specified pin.
*
* @param pin Pin to toggle
* @param value PWM value
*/
void analogWrite(uint8_t pin, uint16_t value)
{
	// Check registry for pin
	std::map<uint8_t, PinModes>::iterator it = vard::pinregistry.find(pin);
	if (it == vard::pinregistry.end())
	{
		vard::logevent(vard::Level::ERR, "analogWrite called. Pin not registered. pin=%u", pin);
	}
	else if (vard::pinregistry.at(pin) == PinModes::OUTPUT)
	{
		vard::logevent(vard::Level::INFO, "analogWrite called. pin=%u value=%u", pin, value);
	}
	else
	{
		vard::logevent(vard::Level::ERR, "analogWrite called. Pin not set for OUTPUT. pin=%u", pin);
	}
}

/**
* Reads a pulse (either HIGH or LOW) on a pin. For example,
* if level is HIGH, pulseIn() waits for the pin to go from
* LOW to HIGH, starts timing, then waits for the pin to go
* LOW and stops timing. Returns the length of the pulse in
* microseconds.
* 
* Description from arduino.cc
* 
* @param pin Pin number to read pulse from.
* @param level Type of pulse to read.
* @return Time in milliseconds.
*/
unsigned long pulseIn(uint8_t pin, OutputLevel level)
{
	while (digitalRead(pin) != level);
	unsigned long start = millis();
	while (digitalRead(pin) == level);
	return millis() - start;
}

/**
*	Reads a word from flash memory. 
* 
*	@param cstr Pointer to a cstring saved in flash memory.
*/
const char* pgm_read_word(const char* const* cstr)
{
	return *cstr;
}

/**
*	Copies the string from src to dest. 
* 
*	@param dest Const char array to write to.
*	@param src C string to copy.
*/
void strcpy_P(char* dest, const char* src)
{
	int i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i++];
	}
	dest[i] = '\0';
}

/**
* Constructor for class String expansion of std::string.
*
*/
String::String(void)
{
	this->str = std::string();
}

/**
* Constructor for class String.
* 
* @param cstr c String.
*/
String::String(const char* cstr)
{
	this->str = std::string(cstr);
}

/**
*	Constructor for class string. 
* 
*	@param number Floating point number to convert to string.
*	@param radix Radix of number.
*/
String::String(float number, Radix radix)
{
	// Only DEC makes sense
	switch (radix)
	{
	case HEX:
	{
		char buff[32];
		sprintf_s(buff, 32, "%a", number);
		this->str = std::string(buff);
		break;
	}

	default:
	{
		char buff[32];
		sprintf_s(buff, 32, "%f", number);
		this->str = std::string(buff);
		break;
	}
	}

}

/**
* Constructor for class String.
* 
* @param num Number to create String out of.
* @param r Radix of number.
*/
String::String(int num, Radix r)
{
	switch (r)
	{
		case Radix::HEX:
		{ // local scope
			char buff[32];
			sprintf_s(buff, 32, "%x", num);
			this->str = std::string(buff);
		}
		break;

		case Radix::OCT:
		{ // local scope
			char buff[32];
			sprintf_s(buff, 32, "%o", num);
			this->str = std::string(buff);
		}
		break;

		default:
			char buff[32];
			sprintf_s(buff, 32, "%d", num);
			this->str = std::string(buff);
		break;
	}
}

/**
* 
* 
* 
* 
*/
String::String(unsigned int num, Radix r)
{
	switch (r)
	{
	case Radix::HEX:
	{ // local scope
		char buff[32];
		sprintf_s(buff, 32, "%ux", num);
		this->str = std::string(buff);
	}
	break;

	case Radix::OCT:
	{ // local scope
		char buff[32];
		sprintf_s(buff, 32, "%uo", num);
		this->str = std::string(buff);
	}
	break;

	default:
		char buff[32];
		sprintf_s(buff, 32, "%u", num);
		this->str = std::string(buff);
		break;
	}
}

/**
* 
* 
* 
*/
String::String(unsigned long num, Radix r)
{
	switch (r)
	{
	case Radix::HEX:
	{ // local scope
		char buff[32];
		sprintf_s(buff, 32, "%lux", num);
		this->str = std::string(buff);
	}
	break;

	case Radix::OCT:
	{ // local scope
		char buff[32];
		sprintf_s(buff, 32, "%luo", num);
		this->str = std::string(buff);
	}
	break;

	default:
		char buff[32];
		sprintf_s(buff, 32, "%lu", num);
		this->str = std::string(buff);
		break;
	}
}

/**
* Returns the length of the String.
* 
* @return 
*/
unsigned int String::length(void)
{
	return (unsigned int) str.length();
}

/**
* Returns a char array of specifed length.
*
* @param buff Buffer to write char length to
* @param size Size of buffer
*/
void String::toCharArray(char* buff, unsigned int size)
{
	std::string subs = this->str.substr(0, size-2); // Leave room for \0
	for (unsigned int i = 0; i < size; i++)
	{
		buff[i] = subs.at(i);
	}
	buff[size - 1] = '\0';
}

/**
* Returns a c string of the String.
* 
* @return pointer to the c string.
*/
const char* String::c_str(void)
{
	return this->str.c_str();
}

/**
*	Returns a character at a given index in the string. 
* 
*	@param i Index of character to return.
*/
char String::charAt(unsigned int i)
{
	return str[i];
}

String String::operator+(String b)
{
	char* buff = new char[this->str.length() + b.length() + 1];
	for (unsigned int i = 0; i < str.length(); i++)
	{
		buff[i] = str[i];
	}

	for (unsigned int i = 0; i < b.length(); i++)
	{
		buff[i + str.length()] = b.charAt(i);
	}

	buff[str.length() + b.length()] = '\0';

	String result = String(buff);
	delete[] buff;

	return result;
}

String String::operator+(const char* b)
{
	return operator+(String(b));
}

/**
* Return std::string of String. 
* 
* @return
*/
std::string String::std_string(void)
{
	return this->str;
}

/**
*	Constructor for class input pin. 
*
*	@param pin Number of pin.
*/
vard::DigitalInputPin::DigitalInputPin(uint8_t pin, OutputLevel (*eventHandler)(void))
{
	this->pin = pin;
	this->eventHandler = eventHandler;
}

/**
*	Fetches digital state of virtual pin.
* 
*	@return Digital state of virtual pin.
*/
OutputLevel vard::DigitalInputPin::read(void)
{
	return (*eventHandler)();
}

/**
*	Getter for the pin number of input pin. 
* 
*	@return Pin number of input pin.
*/
uint8_t vard::DigitalInputPin::getNumber(void)
{
	return pin;
}

/**
*	Attach a virtual digital input pin to the Arduino.
*
*	@param pin Digital Input pin to attach.
*/
void vard::attach_digital_input_pin(vard::DigitalInputPin* pin)
{
	digital_input_pin_registry.insert(std::pair<uint8_t, DigitalInputPin*>(pin->getNumber(), pin));
}

/**
*	Detach a virtual digital input piin from the Arduino.
*
*	@param pin Digital input pin to detach.
*/
void vard::detach_digital_input_pin(vard::DigitalInputPin* pin)
{
	digital_input_pin_registry.erase(pin->getNumber());
}

/*
* Constructor method for class HardwareSerial.
*
*/
vard::HardwareSerial::HardwareSerial(void)
{
	
}

/**
* Destructor for class HardwareSerial.
*
*/
vard::HardwareSerial::~HardwareSerial(void)
{
	if (this->readbuffer != NULL)
	{
		free(this->readbuffer);
	}
}

/*
* Begins serial communication.
*
* Returns 1 if successful and 0 if failed.
*
* @param baud - Baud rate of serial communication
* @return
*/
bool vard::HardwareSerial::begin(unsigned long baud)
{
	if (this->isavailable)
	{
		vard::logevent(vard::Level::INFO, "Serial.begin called: Baud set to %lu.", baud);

		this->isconnected = true;
		return true;
	}
	else
	{
		vard::logevent(vard::Level::ERR, "Serial.begin called: No device available.");

		return false;
	}
}

/*
* Flushes outgoing serial buffer.
*
*/
void vard::HardwareSerial::flush(void)
{
	vard::logevent(vard::Level::INFO, "Serial.flush called.");
}

/**
* Print a message to Serial.
*
* @param text Text to print to Serial.
*/
void vard::HardwareSerial::print(const char* text)
{
	if (!this->isconnected)
	{
		vard::logevent(vard::Level::ERR, "Serial.print called. Port not open.");
		return;
	}
	vard::logevent(vard::Level::INFO, "Serial.print called: %s", text);
}

/**
* Print a message over serial ended by a newline character.
*
* @param text The message to print.
*/
void vard::HardwareSerial::println(const char* text)
{
	if (!this->isconnected)
	{
		vard::logevent(vard::Level::ERR, "Serial.println called. Port not open.");
		return;
	}
	vard::logevent(vard::Level::INFO, "Serial.println called: %s", text);
}

/**
* Prints a String to Serial.
*
* @param str String to print.
*/
void vard::HardwareSerial::print(String str)
{
	print(str.c_str());
}

/**
* Prints a String to Serial with a newline character.
*
* @param str String to print.
*/
void vard::HardwareSerial::println(String str)
{
	println(str.c_str());
}

/**
* Prints a number to Serial in given radix.
*
* @param num Number to print.
* @param radix Radix to print in i.e. DEC.
*/
void vard::HardwareSerial::print(int16_t num, Radix radix)
{
	if (!this->isconnected)
	{
		vard::logevent(vard::Level::ERR, "Serial.print called. Port not open.");
		return;
	}
	char buff[32];
	switch (radix)
	{
		case OCT:
			sprintf_s(buff, 32, "%o", num);
		break;

		case HEX:
			sprintf_s(buff, 32, "%x", num);
		break;

		default:
			sprintf_s(buff, 32, "%d", num);
		break;
	}

	this->print(buff);
}

/**
* Prints a number to Serial in given radix.
*
* @param num Number to print.
* @param radix Radix to print in i.e. DEC.
*/
void vard::HardwareSerial::println(int16_t num, Radix radix)
{
	if (!this->isconnected)
	{
		vard::logevent(vard::Level::ERR, "Serial.println called. Port not open.");
		return;
	}
	char buff[32];
	switch (radix)
	{
	case OCT:
		sprintf_s(buff, 32, "%o", num);
		break;

	case HEX:
		sprintf_s(buff, 32, "%x", num);
		break;

	default:
		sprintf_s(buff, 32, "%d", num);
		break;
	}

	this->println(buff);
}
/**
* Writes a byte to Serial.
* 
* @param val Value of byte to write.
*/
void vard::HardwareSerial::write(uint8_t val)
{
	if (this->isconnected)
	{
		vard::logevent(vard::Level::INFO, "Wire.write called. value=%u", val);
	}
	else
	{
		vard::logevent(vard::Level::ERR, "Serial.write called. Port not open.");
	}
}

/**
* Write bytes from buffer to Serial.
*
* @param buff Byte buffer to write from.
* @param size Size of byte buffer
*/
void vard::HardwareSerial::write(uint8_t* buff, unsigned int size)
{
	if (this->isconnected)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			this->write(buff[i]);
		}
	}
	else
	{
		vard::logevent(vard::Level::ERR, "Serial.write called. Port not open.");
	}
}

/**
* Returns the next byte in the incomming byte buffer.
* 
* @return
*/
uint8_t vard::HardwareSerial::read(void)
{
	if (this->isconnected)
	{
		if (this->pos > this->buffsize)
		{
			vard::logevent(vard::Level::INFO, "Serial.read called. value=%u", this->readbuffer[pos]);
			return this->readbuffer[pos++];
		}
		else
		{
			vard::logevent(vard::Level::ERR, "Serial.read called. readbuffer empty");
			return 0;
		}
	}
	
	vard::logevent(vard::Level::ERR, "Serial.read called. Port not open.");
	return 0;
}

/**
* Read bytes from incomming byte buffer.
* 
* @param buff Byte buffer to receive data.
* @param len Number of bytes to read. Must be less
*	then Serial.available().
*/
void vard::HardwareSerial::readBytes(uint8_t* buff, unsigned int len)
{
	if (this->isconnected)
	{
		for (unsigned int i = 0; i < len; i++)
		{
			buff[i] = this->read();
		}
	}
	else
	{
		vard::logevent(vard::Level::ERR, "Serial.readBytes called. Port not open.");
	}
}

/**
* Reads a String from Serial.
* 
* @return String object.
*/
String vard::HardwareSerial::readString(void)
{
	if (this->isconnected)
	{
		unsigned int len = this->buffsize - this->pos;
		char* word = (char*)malloc(static_cast<size_t>(len) + 1);
		if (word != NULL)
		{
			for (unsigned int i = this->pos; i < this->buffsize; i++)
			{
				word[i - this->pos] = (char)this->read();
			}
			word[len] = '\0';
			String str = String(word);
			free(word);
			return str;
		}
		vard::logevent(vard::Level::ERR, "Serial.readString called. Faild to allocate space.");
		return "";
	}

	vard::logevent(vard::Level::ERR, "Serial.readString called. Port not open.");
	return String();
}

/**
* Returns size of incomming byte buffer.
* 
* @return
*/
unsigned int vard::HardwareSerial::available(void)
{
	return this->buffsize - this->pos;
}

/**
* Set the timeout of the serial connection.
*
* @param delay Maximum timeout in milliseconds.
*/
void vard::HardwareSerial::setTimeout(unsigned long delay)
{
	vard::logevent(vard::Level::INFO, "Serial.setTimeout called. value=%lu", delay);
}

/**
*	Initializes a new I2C device. 
* 
*	@param address Address of the I2C device.
*/
vard::I2C_Device::I2C_Device(uint8_t address, void (*eventHandler)(uint8_t*, unsigned int))
{
	this->address = address;
	this->eventHandler = eventHandler;
}

/**
*	Getter for the i2c device's address. 
* 
*	@return Address of the device.
*/
uint8_t vard::I2C_Device::getAddress(void)
{
	return address;
}

/**
*	Takes over data sent by the Arduino to the i2c device.
*	
*	@param buffer Array containing sent/recieved bytes.
*/
void vard::I2C_Device::recieve(uint8_t* buffer, unsigned int size)
{
	// Invoke user defined event handler method
	(*eventHandler)(buffer, size);
	delete[] buffer; // Prevent memory leak
}

/**
*	Prompts the i2c device to empty its write buffer by
*	transmitting the data to the Arduino.
* 
*	@param size Amount of bytes requested.
*	@return Array of given size read from the devices writebuffer.
*/
std::queue<uint8_t>* vard::I2C_Device::transmit(unsigned int size)
{
	std::queue<uint8_t>* buffer = new std::queue<uint8_t>;
	unsigned int buffersize = writebuffer.size();
	for (int i = 0; i < size && i < buffersize; i++)
	{
		buffer->push(writebuffer.front());
		writebuffer.pop();
	}
	writebuffer = std::queue<uint8_t>(); // clear i2c device writebuffer
	return buffer;
}

/**
*	Adds a byte to the writebuffer of the device. 
* 
*	@param byte The byte to add to the buffer.
*/
void vard::I2C_Device::push(uint8_t byte)
{
	writebuffer.push(byte);
}

/**
*	Attach a virtual i2c device to the Arduino. 
* 
*	@param device Pointer to the device to attach.
*/
void vard::attach_I2C_Device(vard::I2C_Device* device)
{
	i2c_device_registry.insert(std::pair<uint8_t, I2C_Device*>(device->getAddress(), device));
}

/**
*	Detach a virtual i2c device from the Arduino. 
* 
*	@param device Pointer to the device to detach.
*/
void vard::detach_I2C_Device(vard::I2C_Device* device)
{
	i2c_device_registry.erase(device->getAddress());
}