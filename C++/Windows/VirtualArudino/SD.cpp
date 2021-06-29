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
* SD.cpp - Source code for virtual SD card. Part of the Virtual Arduino runtime
*	environment.
*
*/

#include "SD.h"

/*
* Constructor method for class Sd2Card.
*
*/
Sd2Card::Sd2Card(void)
{

}

/** 
* Destructor for class Sd2Card.
*
*/
Sd2Card::~Sd2Card(void)
{

}

/**
*	Initializes Sd2Card. 
* 
*	@param speed W/R speed selection of chip.
*	@param cs Chip select number.
*/
uint8_t Sd2Card::init(int speed, int cs)
{
	vard::logevent(vard::Level::INFO, "SD2Card.init called. speed=%d cs=%d", speed, cs);
	return true;
}

/**
*	Returns Sd card type. 
* 
*/
uint8_t Sd2Card::type(void)
{
	return SD_TYPE;
}

/*
* Constructor method for class SdVolume.
*
*/
SdVolume::SdVolume(void)
{

}

/**
* Destructor for class SdVolume.
*
*/
SdVolume::~SdVolume(void)
{

}

/**
*	Initializes SdVolume. 
* 
*	@sd SD card volume is found on.
*/
uint8_t SdVolume::init(Sd2Card* sd)
{
	vard::logevent(vard::Level::INFO, "SdVolume.init called.");
	return true;
}

/**
*	Returns SD volume cluster count. 
* 
*/
unsigned int SdVolume::clusterCount(void)
{
	return SD_CLUSTER_COUNT;
}

/**
*	Returns SD volume blocks per cluster count. 
* 
*/
unsigned int SdVolume::blocksPerCluster(void)
{
	return SD_BLOCKS_PER_CLUSTER;
}

/**
*	Returns SD volume FAT type. 
*
*/
unsigned int SdVolume::fatType(void)
{
	return SD_FAT_TYPE;
}

/*
* Constructor method for class SdFile.
*
*/
SdFile::SdFile(void)
{

}

/**
* Destructor for class SdFile.
*
*/
SdFile::~SdFile(void)
{

}

/**
*	Opens root directory of Sd volume. 
* 
*	@param vol Sd Volume.
*	@return True if successfull.
*/
uint8_t SdFile::openRoot(SdVolume* vol)
{
	if (vol->rootPath != NULL)
	{
		vard::logevent(vard::Level::INFO, "SdFile.openRoot called.");
		path = vol->rootPath;
		return true;
	}
	
	vard::logevent(vard::Level::ERR, "SdFile.openRoot called. SdVolume rootPath = NULL.");
	return false;
}

/**
*	Creates a new directory with given name.
* 
*	@param dir Root directory to create new dir in.
*	@param dirname Name of new directory.
*	@return True if successfull.
*/
uint8_t SdFile::makeDir(SdFile* dir, const char* dirname)
{
	if (dir->path != NULL)
	{
		char* path = new char[strlen(dir->path) + strlen(dirname) + 1]; // +1 for \0
		strcpy_s(path, strlen(dir->path) + 1, dir->path);
		strcpy_s(path + strlen(dir->path), strlen(dirname) + 1, dirname); // \0 included
		
		bool result = CreateDirectoryA(path, NULL);
		delete[] path;

		if (result)
		{
			vard::logevent(vard::Level::INFO, "SdFile.makeDir called. Dir created. dirname=%s", dirname);
		}
		else
		{
			vard::logevent(vard::Level::ERR, "SdFile.makeDir called. Failed to create new dir. dirname=%s", dirname);
		}
		return result;
	}

	vard::logevent(vard::Level::ERR, "SdFile.makeDir called. SdFile path = NULL.");
	return false;
}

/**
*	Opens/creates a file with given name.
* 
*	@param dir Root directory of file.
*	@param fname Name of file to open.
*	@param mode Open mode.
*	@return Returns true (1) if successful.
*/
uint8_t SdFile::open(SdFile* dir, const char* fname, uint8_t mode)
{
	// Check to see if fname is a directory, concatenate path with fname
	char* path = new char[strlen(dir->path) + strlen(fname) + 1]; // +1 for \0
	strcpy_s(path, strlen(dir->path) + 1, dir->path);
	strcpy_s(path + strlen(dir->path), strlen(fname) + 1, fname); // \0 included

	DWORD result = GetFileAttributesA(path);
	if (result & FILE_ATTRIBUTE_DIRECTORY && result != INVALID_FILE_ATTRIBUTES)
	{
		// File is a directory, append \\ to the end of the path
		char* newpath = new char[strlen(path) + 2]; // +1 for \\ and +1 for \0
		strcpy_s(newpath, strlen(path) + 1, path);
		newpath[strlen(path)] = '\\';
		newpath[strlen(path) + 1] = '\0';
		delete[] path;

		this->path = newpath;

		vard::logevent(vard::Level::INFO, "SdFile.open called. File is a dir. fname=%s mode=%u", fname, mode);
		return 1;
	}

	// Set file path and create file
	this->path = dir->path;
	fopen_s(&file, path, "w");

	// Copy fname
	this->fname = new char[strlen(fname) + 1];
	strcpy_s((char*) this->fname, strlen(fname) + 1, fname);

	// Check if file successfully created
	if (file != NULL)
	{
		vard::logevent(vard::Level::INFO, "SdFile.open called. File opened. fname=%s mode=%u", fname, mode);
		return 1;
	}

	vard::logevent(vard::Level::ERR, "SdFile.open called. Cannot open file. fname=%s mode=%u", fname, mode);
	return 0;
}

uint8_t SdFile::open(SdFile dir, const char* fname, uint8_t mode)
{
	return open(&dir, fname, mode);
}

/**
*	Writes a single char to the file. 
* 
*	@param c Char to write.
*	@return Returns true (1) if successful and false (0) if fails.
*/
uint8_t SdFile::write(char c)
{
	// Create string out of char
	return fputc(c, file) != EOF ? 1 : 0;
}

/**
*	Writes a c string to the file.
*
*	@param str String to write.
*	@return Returns true (1) if successful and 0 if fails.
*/
uint8_t SdFile::write(const char* str)
{
	// Check to see if file is open
	if (file != NULL)
	{
		if (fputs(str, file) == EOF)
		{
			vard::logevent(vard::Level::ERR, "SdFile.write called. Failed to write to file.");
			return 0;
		}
		
		/*char* buff = new char[strlen(str) + 32];
		sprintf_s(buff, strlen(str) + 32, "SdFile.write called: %s", str);
		vard::logevent(vard::Level::INFO, buff);
		delete[] buff;*/

		return 1;
	}

	vard::logevent(vard::Level::ERR, "SdFile.write called. File not open.");

	return 0;
}

/**
*	Flushes buffer before continuing.
*
*/
void SdFile::sync(void)
{
	// Check to see if file exists
	if (file != NULL)
	{
		fflush(file);
		vard::logevent(vard::Level::INFO, "SdFile.sync called. fname=%s", fname);
	}
	else
	{
		vard::logevent(vard::Level::WARNING, "SdFile.sync called. File not open.");
	}
}

/**
*	Closes the Sd file. 
* 
*/
void SdFile::close(void)
{
	// Check to see if file exists
	if (file != NULL)
	{
		fclose(file);
		vard::logevent(vard::Level::INFO, "SdFile.close called. fname=%s", fname);

		file = NULL;
		delete[] fname;
	}
	else
	{
		vard::logevent(vard::Level::WARNING, "SdFile.close called. File not open.");
	}
}