#pragma once
#include <cstdint>
#include <string>
#ifndef ARDUINO
#include <fstream>
#else
#include <SD.h>
#endif
using namespace std;

enum FileMode
{
#ifndef ARDUINO
	Read = fstream::in | fstream::binary,
	Write = Read | fstream::out,
	WriteAppend = Write | fstream::app,
#else
	Read = FILE_READ,
	Write = Read | FILE_WRITE,
	WriteAppend = Write | FILE_WRITE,
#endif
};

class FileReader
{
public:
	FileReader(string fileName, FileMode mode = FileMode::Read);
	~FileReader();

	streampos position();
	streampos start();
	streampos end();
	void seek(streampos position);
	void seek(streamoff offset, streampos from);
	streamoff length();
	bool readLine(string& line);
	bool readToEnd(string &content);
	string fileName;
	FileMode mode;

protected:
	virtual bool _readLine(string& line);

	virtual void open();
#ifdef ARDUINO
	SDLib::File file;
#else
	fstream file;
#endif
	void log(string str)
	{
	#ifndef ARDUINO
		printf((str + "\n").c_str());
	#else
		Serial.println(str.c_str());
	#endif
	}
};

class FileWriter : public FileReader
{
public:
	FileWriter(string fileName, FileMode mode = FileMode::WriteAppend);
	~FileWriter();

	void write(const string& text);
protected:
	//virtual void open();
};

