#include "StringUtilities.h"
#include <string>
#include <sstream>
#include <vector>

StringUtilities::StringUtilities()
{

}

StringUtilities::~StringUtilities()
{

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
