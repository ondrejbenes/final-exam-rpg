#include "ResourceLoader.h"
#include "Logger.h"

ResourceLoader::ResourceLoader()
{
	if (!_menuFont.loadFromFile("Resources/Fonts/sans.ttf"))
		LOG_E("Could not menu load font");

	if (!_consoleFont.loadFromFile("Resources/Fonts/sans.ttf"))
		LOG_E("Could not menu load font");
}

ResourceLoader::~ResourceLoader() 
{
	for (auto texture : _textures)
		delete texture.second;
}

std::shared_ptr<ResourceLoader> ResourceLoader::getInstance()
{
	if (instance == nullptr)
		instance = std::make_shared<ResourceLoader>(ResourceLoader());
	return instance;
}

sf::Texture* ResourceLoader::getTexture(std::string filePath)
{
	if (_textures.find(filePath) != end(_textures))
		return _textures[filePath];

	auto texture = new sf::Texture;
	if (!texture->loadFromFile(filePath))
		LOG_E("Error loading texture: " + filePath);
	_textures[filePath] = texture;
	return _textures[filePath];
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