#include "TimeUtilities.h"
#include <iomanip>
#include <chrono>
#include <sstream>
#include <Windows.h>


std::string TimeUtilities::getCurrentDate()
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	std::stringstream ss;

	ss << 
		st.wDay << ". " <<
		st.wMonth << ". " <<
		st.wYear;

	return ss.str();
}

std::string TimeUtilities::getCurrentTime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	std::stringstream ss;

	ss << 
		std::setfill('0') <<
		std::setw(2) << st.wHour << ":" <<
		std::setw(2) << st.wMinute << ":" <<
		std::setw(2) << st.wSecond << "." <<
		std::setw(3) << st.wMilliseconds;

	return ss.str();
}
