#pragma once
#include <cstdint>
#include <string>
#include <fstream>
using namespace std;

enum FileMode
{
	Read = fstream::in | fstream::binary,
	Write = Read | fstream::out,
	WriteAppend = Write | fstream::app,
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
	SDlib::File file;
#else
	fstream file;
#endif
	void log(string str) { printf((str + "\n").c_str()); }
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

