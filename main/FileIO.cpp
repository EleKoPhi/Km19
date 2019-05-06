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
	file.write(text.c_str());
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

streamposition FileReader::position()
{
#ifndef ARDUINO
	return file.tellg();
#else
	return file.position();
#endif
}

void FileReader::seekStart()
{
#ifndef ARDUINO
	seek(file.beg);
#else
	seek(0);
#endif
}

void FileReader::seekEnd()
{
#ifndef ARDUINO
	seek(file.end);
#else
	seek(length());
#endif
}

void FileReader::seek(streamposition position)
{
#ifndef ARDUINO
	file.seekg(position);
	file.seekp(position);
#else
	file.seek(position);
#endif
}

void FileReader::seek(streamoffset offset, streamposition from)
{
#ifndef ARDUINO
	file.seekg(offset, from);
	file.seekp(offset, from);
#else
	file.seek(offset + from);
#endif
}

streamoffset FileReader::length()
{
#ifndef ARDUINO
	streamposition backup = file.tellg();
	file.seekg(0, file.end);
	streamposition length = file.tellg();
	file.seekg(0, backup);
	return length;
#else
	return file.size();
#endif
}

bool FileReader::readToEnd(string & content)
{
#ifndef ARDUINO
	std::stringstream buffer;
	buffer << file.rdbuf();
	content = buffer.str();
	return true;
#else
	content = "";
	while(file.available())
	{
		content = content + (char)file.read();
	}
	return true;
#endif
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
#ifdef ARDUINO
	UserHandler::sdCard();
	file = SD.open(fileName.c_str(), mode);
	if(!file)
	{
		//string status;
		//int flags = file.rdstate();
		//if (flags & eofbit)
		log("file.open('" + fileName + "', " + to_string(mode) + ") failed.");
	}
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
