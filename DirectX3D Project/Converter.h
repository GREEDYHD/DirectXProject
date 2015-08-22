#pragma once
#ifndef _CONVERTER_H
#define _CONVERTER_H
#include <string>
#include <sstream>

using namespace std;
struct Converter
{
	static const wchar_t* ToWChar(string _String)
	{
		wstringstream Temp;
		Temp << _String.c_str();
		const wchar_t* Conversion = Temp.str().c_str();
		return Conversion;
	}
};
#endif