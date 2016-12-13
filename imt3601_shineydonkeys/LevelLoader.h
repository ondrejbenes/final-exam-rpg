#pragma once

#include <string>
#include "tinyxml2.h"

class LevelLoader
{
public:
	void load(const std::string& definitionXml) const;
private:
	void loadMap(const tinyxml2::XMLDocument& doc) const;
	void loadKeys(const tinyxml2::XMLDocument& doc) const;
	void loadWeapons(const tinyxml2::XMLDocument& doc) const;
	void loadPlayer(const tinyxml2::XMLDocument& doc) const;
	void loadNpcs(const tinyxml2::XMLDocument& doc) const;
	void loadDonkey() const;
};

