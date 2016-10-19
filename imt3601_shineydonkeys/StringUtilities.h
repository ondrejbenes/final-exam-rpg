#pragma once

#include <vector>

class StringUtilities
{
public:
	static std::string doubleToBinaryStr(double value);

	// Taken from http://stackoverflow.com/questions/236129/split-a-string-in-c
	static std::vector<std::string> split(const std::string &s, char delim);

	// Taken from http://stackoverflow.com/questions/8095088/how-to-check-string-start-in-c
	static bool startsWith(std::string& str, std::string& start);
private:
	static void split(const std::string &s, char delim, std::vector<std::string> &elems);
};

