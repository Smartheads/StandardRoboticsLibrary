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
* SD.h - Header file of SD source code. Part of the Virtual Arduino runtime
*	enviroment.
*
*/

#ifndef __SD_H__
#define __SD_H__

#include "VirtualArduino.h"

/* SD Card properties */
#define SD_TYPE 3

/* SD Volume properties */
#define SD_CLUSTER_COUNT 979712
#define SD_BLOCKS_PER_CLUSTER 8
#define SD_FAT_TYPE 32

#define O_READ 0x01 // oflag for read
#define O_RDONLY O_READ // same as O_READ
#define O_WRITE 4	// oflag for write
#define O_WRONLY O_WRITE // same as O_WRITE
#define O_RDWR (O_READ | O_WRITE) // open() oflag for reading and writing
#define O_ACCMODE (O_READ | O_WRITE) // open() oflag mask for access modes
#define O_APPEND 0x04 // offset shall be set to the end of the file prior to each write
#define O_SNYC 0x08 // synchronous write - call sync() after each write
#define O_CREAT 0x10 // create the file if nonexistent
#define O_EXCL 0x20	// if O_CREAT and O_EXCL are set, open() shall fail if the file exists
#define O_TRUNC 0x40 // truncate file to zero length

#define SPI_FULL_SPEED 0
#define SPI_HALF_SPEED 1
#define SPI_QUATER_SPEED 2

class Sd2Card
{
	public:
		Sd2Card(void);
		~Sd2Card(void);

		uint8_t init(int, int);
		uint8_t type(void);
};

class SdVolume
{
public:
	SdVolume(void);
	~SdVolume(void);

	uint8_t init(Sd2Card*);
	unsigned int clusterCount(void);
	unsigned int blocksPerCluster(void);
	unsigned int fatType(void);
private:
	friend class SdFile;
	const char* rootPath = vard::sdvolume_root;
};

class SdFile
{
public:
	SdFile(void);
	~SdFile(void);

	uint8_t openRoot(SdVolume*);
	uint8_t makeDir(SdFile*, const char*);
	uint8_t open(SdFile*, const char*, uint8_t);
	uint8_t open(SdFile, const char*, uint8_t);
	uint8_t write(char);
	uint8_t write(const char*);
	void sync(void);
	void close(void);

private:
	const char* path = NULL;
	FILE* file = NULL;
	const char* fname = NULL;
};

#endif