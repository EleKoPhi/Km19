#include "stdafx.h"
#include "Helper.h"



#ifdef ARDUINO
inline std::string to_string(int value)
{
	String ard(value, DEC);
	std::string text(ard.begin());
	return text;
}
int stoi(std::string text)
{
	return String(text.c_str()).toInt();
}
double stod(std::string text)
{
	return String(text.c_str()).toDouble();
}
#endif