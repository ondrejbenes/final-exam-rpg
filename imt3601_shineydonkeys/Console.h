#pragma once

#include <string>

class Console
{

public:
	Console();
	virtual ~Console();

	int handleInput(std::string input);

};