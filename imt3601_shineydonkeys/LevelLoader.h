#pragma once

#include <string>

class LevelLoader
{
public:
	LevelLoader();
	
	void load(const std::string& definitionXml) const;
};

