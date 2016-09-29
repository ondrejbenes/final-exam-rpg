#pragma once
#include <exception>

class InvalidEngineStateException : public std::exception
{
public:
	InvalidEngineStateException() {}
	~InvalidEngineStateException() {}
	const char* what() const throw() override;
};

