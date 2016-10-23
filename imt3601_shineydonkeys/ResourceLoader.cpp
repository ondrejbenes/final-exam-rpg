#include "ResourceLoader.h"
#include "Logger.h"


ResourceLoader::ResourceLoader()
{
	if (!_menuFont.loadFromFile("Resources/Fonts/sans.ttf"))
		LOG_E("Could not load font");
}


ResourceLoader* ResourceLoader::getInstance()
{
	if (instance == nullptr)
		instance = new ResourceLoader;
	return instance;
}

ResourceLoader::~ResourceLoader()
{

}

const sf::Font& ResourceLoader::getMenuFont()
{
	return _menuFont;
}

ResourceLoader* ResourceLoader::instance = nullptr;