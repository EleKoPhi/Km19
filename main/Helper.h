#pragma once
#ifndef Helper_h
#define Helper_h

#include "defines.h"

#include "Arduino.h"
#undef min
#undef max

#include <string>
using namespace std;

//#ifdef ARDUINO

std::string to_string(int value);

int stoi(std::string text);
double stod(std::string text);

class Exception : public exception
{
	std::string msg;
public:
	Exception(std::string what) : exception(), msg(what) {}
	virtual ~Exception() {}

	virtual const char* what() const throw()
	{
		return msg.c_str();
	}
};

//#endif

#endif
