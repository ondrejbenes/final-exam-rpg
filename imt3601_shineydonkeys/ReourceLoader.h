#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ReourceLoader
{

public:
	ReourceLoader();
	virtual ~ReourceLoader();
	static sf::Texture PLAYER_SPRITE_SHEET;
	static sf::SoundBuffer THEME_SONG;

};
