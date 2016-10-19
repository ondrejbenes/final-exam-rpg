#include "StringUtilities.h"
#include <string>
#include <sstream>
#include <vector>

std::string StringUtilities::doubleToBinaryStr(double value)
{
	auto doubleAsString = std::to_string(value);
	auto beforeDecimalPointStr = doubleAsString.substr(doubleAsString.length() - doubleAsString.find('.'));

	std::string afterDecimalPointStr = "";

	double afterDecimalPoint = 2 * (abs(value) - abs((long)value));
	for (int i = 0; i < 15; i++)
	{
		if (afterDecimalPoint >= 1)
		{
			afterDecimalPointStr += "1";
			if (abs(afterDecimalPoint - 1) < pow(10, -8)) 
				break;
			afterDecimalPoint -= 1;
		}
		else
		{
			afterDecimalPointStr += "0";
		}
		afterDecimalPoint *= 2;
	}

	return beforeDecimalPointStr + "." + afterDecimalPointStr;
}

std::vector<std::string> StringUtilities::split(const std::string &s, char delim) 
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

bool StringUtilities::startsWith(std::string& str, std::string& start)
{
	return str.compare(0, start.length(), start) == 0;
}

void StringUtilities::split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}