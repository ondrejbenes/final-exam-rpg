#include "ResourceLoader.h"
#include "Logger.h"

ResourceLoader::ResourceLoader()
{
	if (!_menuFont.loadFromFile("Resources/Fonts/sans.ttf"))
		LOG_E("Could not menu load font");

	if (!_consoleFont.loadFromFile("Resources/Fonts/sans.ttf"))
		LOG_E("Could not menu load font");
}

std::shared_ptr<ResourceLoader> ResourceLoader::getInstance()
{
	if (instance == nullptr)
		instance = std::make_shared<ResourceLoader>(ResourceLoader());
	return instance;
}

const sf::Font& ResourceLoader::getMenuFont() const
{
	return _menuFont;
}

const sf::Font& ResourceLoader::getConsoleFont() const
{
	return _consoleFont;
}

std::shared_ptr<ResourceLoader> ResourceLoader::instance = nullptr;