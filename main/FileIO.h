#pragma once
#include <string>
#ifndef ARDUINO
#include <cstdint>
#include <fstream>
#else
#include <SD.h>
#endif
using namespace std;

#ifndef ARDUINO
typedef streampos streamposition;
typedef streamoff streamoffset;
#else
typedef uint32_t streamposition;
typedef uint32_t streamoffset;
#endif

enum FileMode : uint8_t
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

	streamposition position();
	void seekStart();
	void seekEnd();
	void seek(streamposition position);
	void seek(streamoffset offset, streamposition from);
	streamoffset length();
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

void renameFile(string from, string to)
{

}
