#include "Logger.h"
#include "TimeUtilities.h"
#include <fstream>
#include <sstream>
#include <iostream>

void Logger::log(const char severity, std::string callerClass, const char* callerFunction, int line, std::string message)
{
	std::stringstream ss;
	ss << 
		TimeUtilities::getCurrentTimeAndDate() << " " << 
		severity << " " << 
		callerClass << "::" << 
		callerFunction << "(), line " << 
		line << "\t" << 
		message << "\n";
	std::ofstream ofs;
	ofs.open("log.txt", std::ofstream::app);
	ofs << ss.str();
	ofs.close();

	#ifdef _DEBUG
		std::cout << ss.str();
	#endif
}
