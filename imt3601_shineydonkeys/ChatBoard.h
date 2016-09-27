#pragma once

#include <vector>

class ChatBoard
{

public:
	ChatBoard();
	virtual ~ChatBoard();

private:
	std::vector<std::string> messages;

};
