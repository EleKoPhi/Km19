#pragma once
#ifndef Helper_h
#define Helper_h

#include "Arduino.h"
#undef min
#undef max

#include <string>

#ifdef ARDUINO

std::string to_string(int value);

int stoi(std::string text);
double stod(std::string text);

#endif

#endif
