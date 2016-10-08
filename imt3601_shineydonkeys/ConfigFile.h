#pragma once
#include <string>
#include <Windows.h>


// Inspiration: http://www.codeproject.com/Articles/10809/A-Small-Class-to-Read-INI-File
// https://msdn.microsoft.com/en-us/library/s7wzt4be.aspx
// TODO may be useful to change to a class with getters / setters which will use WIN API
struct ConfigFile
{
	ConfigFile()
	{
		resolution_width = GetPrivateProfileInt(L"graphics", L"resolution_width", 640, fileName);
		resolution_height = GetPrivateProfileInt(L"graphics", L"resolution_height", 320, fileName);
		fullscreen = GetPrivateProfileInt(L"graphics", L"fullscreen", 0, fileName);
		vSync = GetPrivateProfileInt(L"graphics", L"vSync", 0, fileName);

		LPWSTR result = new TCHAR[100];;
		GetPrivateProfileString(L"player", L"name", L"Unknown", result, 255, fileName);
		char buffer[100];
		size_t i;
		wcstombs_s(&i, buffer, result, 100);
		playerName = buffer;
		delete[] result;
	}

	unsigned int resolution_width;
	unsigned int resolution_height;
	bool fullscreen;
	bool vSync;

	std::string playerName;
private:
	const wchar_t* fileName = L".\\config.ini";
};
