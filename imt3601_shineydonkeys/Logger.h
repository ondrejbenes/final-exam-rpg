#pragma once

#include <iostream>

#pragma warning( disable : 4996 )

#define __SHORT_FILE__ std::string(__FILE__).substr(std::string(__FILE__).rfind("\\") + 1)
#define LOG_I(message) Logger::log('I', __SHORT_FILE__, __func__, __LINE__, message)
#define LOG_D(message) Logger::log('D', __SHORT_FILE__, __func__, __LINE__, message)
#define LOG_W(message) Logger::log('W', __SHORT_FILE__, __func__, __LINE__, message)
#define LOG_E(message) Logger::log('E', __SHORT_FILE__, __func__, __LINE__, message)

class Logger
{
	static std::ostream& os;
public:
	/**
	 * \brief Should not be called directly. Use one of the macros instead.
	 */
	static void log(const char severity, std::string callerClass, const char* callerFunction, int line, std::string message);
};

