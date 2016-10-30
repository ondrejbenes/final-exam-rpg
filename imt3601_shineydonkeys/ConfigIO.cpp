#include "ConfigIO.h"

#include <Windows.h>
#include <sstream>

int ConfigIO::readInt(const wchar_t* section, const wchar_t* key, const int& defaultValue, const wchar_t* file)
{
	return GetPrivateProfileInt(section, key, defaultValue, file);
}

std::string ConfigIO::readString(const wchar_t* section, const wchar_t* key, const wchar_t* defaultValue, const wchar_t* file)
{
	auto result = new TCHAR[1024];
	char buffer[1024];
	size_t i;
	GetPrivateProfileString(section, key, defaultValue, result, 1024, file);
	wcstombs_s(&i, buffer, result, 1024);
	delete[] result;

	return buffer;
}

void ConfigIO::writeInt(const wchar_t* section, const wchar_t* key, const int& value, const wchar_t* file)
{
	writeString(section, key, std::to_wstring(value).c_str(), file);
}

void ConfigIO::writeString(const wchar_t* section, const wchar_t* key, const wchar_t* value, const wchar_t* file)
{
	auto res = WritePrivateProfileString(section, key, value, file);
}
