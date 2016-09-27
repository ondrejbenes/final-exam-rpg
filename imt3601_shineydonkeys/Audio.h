#pragma once
#include "Module.h"

#include <SFML/Audio.hpp>

class Audio : public Module
{

public:
	Audio();
	virtual ~Audio();

	void playSound(sf::SoundBuffer soundName);
	void playTrack(sf::SoundBuffer trackName);

};
