#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ResourceLoader
{

public:
	static ResourceLoader* getInstance();

	virtual ~ResourceLoader();
	static sf::Texture PLAYER_SPRITE_SHEET;
	static sf::SoundBuffer THEME_SONG;

	const sf::Font& getMenuFont();

private:
	static ResourceLoader* instance;
	ResourceLoader();

	sf::Font _menuFont;
};
