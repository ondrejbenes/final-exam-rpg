#include "Logger.h"
#include "TimeUtilities.h"

std::ostream& Logger::os = std::cout;

void Logger::log(const char severity, std::string callerClass, const char* callerFunction, int line, std::string message)
{
	os << 
		TimeUtilities::getCurrentTimeAndDate() << " " << 
		severity << " " << 
		callerClass << "::" << 
		callerFunction << "(), line " << 
		line << "\t" << 
		message << "\n";
}