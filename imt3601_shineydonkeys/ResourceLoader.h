#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

/**
 * \brief Singleton used for loading resources, such as Fonts or Textures
 */
class ResourceLoader
{
public:
	static std::shared_ptr<ResourceLoader> getInstance();

	/* Return a pointer to texture */
	std::shared_ptr<sf::Texture> getTexture(std::string filePath);

	/* Returns a const reference to the sf::Font used for Menu sf::Texts */
	const sf::Font& getMenuFont() const;

	/* Returns a const reference to the sf::Font used for Console sf::Texts */
	const sf::Font& getConsoleFont() const;
protected:
	ResourceLoader();
private:
	static std::shared_ptr<ResourceLoader> instance;

	std::map<std::string, std::weak_ptr<sf::Texture>> _textures;

	sf::Font _menuFont;
	sf::Font _consoleFont;
};
