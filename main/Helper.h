#pragma once
#ifndef Helper_h
#define Helper_h

#include "defines.h"

#define __cplusplus 201103L
#include "Arduino.h"
#undef min
#undef max

#include <string>
using namespace std;

#ifdef ARDUINO

std::string to_string(int value);

int stoi(std::string text);
double stod(std::string text);

#endif

#endif
