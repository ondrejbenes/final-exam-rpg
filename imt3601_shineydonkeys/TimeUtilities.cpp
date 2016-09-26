#include "TimeUtilities.h"
#include <iomanip>
#include <chrono>
#include <sstream>
#include <Windows.h>

std::string TimeUtilities::getCurrentTimeAndDate()
{
	SYSTEMTIME st;
	GetSystemTime(&st);

	std::stringstream ss;

	ss << std::setfill('0') <<
		st.wDay << ". " <<
		st.wMonth << ". " <<
		st.wYear << " " <<
		st.wHour << ":" <<
		std::setw(2) << st.wMinute << ":" <<
		std::setw(2) << st.wSecond << "." <<
		std::setw(3) << st.wMilliseconds;

	return ss.str();
}
