#include "stdafx.h"
#include "FileIO.h"
#include <sstream>
#include "UserHandler.h"

FileWriter::FileWriter(string fileName, FileMode mode)
	: FileReader(fileName, mode)
{
	//this->open();
}

FileWriter::~FileWriter()
{
#ifdef ARDUINO
	file.close();
#else
	file.close();
#endif
}

FileReader::FileReader(string fileName, FileMode mode)
	: fileName(fileName)
	, mode(mode)
#ifdef ARDUNIO
	, file(SD.open(fileName, FileMode::Read))
#else
	, file()
#endif
{
	this->open();
}

FileReader::~FileReader()
{
#ifdef ARDUINO
	file.close();
#else
	file.close();
#endif
}

void FileWriter::write(const string& text)
{
#ifdef ARDUINO
	file.print(text);
#else
	file.write(text.c_str(), text.length());
	if(!file.good())
	{
		//string status;
		//int flags = file.rdstate();
		//if (flags & eofbit)
		log("file.open('" + fileName + "') failed: " + to_string(file.rdstate()) + ", errno:" + strerror(errno));
	}

#endif
}

streampos FileReader::position()
{
	return file.tellg();
}

streampos FileReader::start()
{
	return (file.beg);
}

streampos FileReader::end()
{
	return (file.end);
}

void FileReader::seek(streampos position)
{
	file.seekg(position);
	file.seekp(position);
}

void FileReader::seek(streamoff offset, streampos from)
{
	file.seekg(offset, from);
	file.seekp(offset, from);
}

streamoff FileReader::length()
{
	streampos backup = file.tellg();
	file.seekg(0, file.end);
	streampos length = file.tellg();
	file.seekg(0, backup);
	return length;
}

bool FileReader::readToEnd(string & content)
{
	std::stringstream buffer;
	buffer << file.rdbuf();
	content = buffer.str();
	return true;
}

bool FileReader::readLine(string & line)
{
	bool result = _readLine(line);
	if(line.length() > 1 && line[line.length() - 1] == '\r')
		line.replace(line.length() - 1, 1, "");
	return result;
}

bool FileReader::_readLine(string& line)
{
	try
	{
	#ifdef ARDUINO
		if(file.available() < 2)
			return false;
		file.setTimeout(20);
		return file.readStringUntil('\n');
	#else
		if(!file.is_open())
			open();
		if(!file.is_open())
			return false;
		if(file.eof())
			return false;
		//if(file >> line)
		if(getline(file, line))
			return true;
	#endif
	}
	catch(exception exc)
	{
		log(exc.what());
	}
	return false;
}

void FileReader::open()
{
#ifdef ARDUNIO
	file.open(SD.open(fileName, mode));
#else
	string path = TestFolder + fileName;
	file.open(path, mode | fstream::out);
	if(!file.good())
	{
		//string status;
		//int flags = file.rdstate();
		//if (flags & eofbit)
		log("file.open('" + path + "') failed: " + to_string(file.rdstate()) + ", errno:" + strerror(errno));
	}
#endif
}
