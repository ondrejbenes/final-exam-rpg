#pragma once

#include <string>

// Inspiration: http://www.codeproject.com/Articles/10809/A-Small-Class-to-Read-INI-File
// https://msdn.microsoft.com/en-us/library/s7wzt4be.aspx

class ConfigIO
{
public:
	static int readInt(const wchar_t* section, const wchar_t* key, const int& defaultValue = 0, const wchar_t* file = L"./Config/config.ini");
	static std::string readString(const wchar_t* section, const wchar_t* key, const wchar_t* defaultValue = L"", const wchar_t* file = L"./Config/config.ini");

	static void writeInt(const wchar_t* section, const wchar_t* key, const int& value, const wchar_t* file = L"./Config/config.ini");
	static void writeString(const wchar_t* section, const wchar_t* key, const wchar_t* value, const wchar_t* file = L"./Config/config.ini");
};