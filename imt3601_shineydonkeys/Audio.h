#pragma once

#include "Module.h"
#include "Sound.h"

#include <map>
#include <memory>

#include <SFML/Audio.hpp>

class Audio : public Module
{
public:
	// SOUNDS
	const static std::string WEAPON_SLASH;
	const static std::string HUMAN_NPC_DYING;

	// MUSIC
	const static std::string THEME_SONG;


	Audio();
	bool initialize() override;
	void update() override;

	void playSound(const std::string& soundName);
	void playMusic(const std::string& musicName, bool loop = true);

	void reloadVolumeFromConfig();
private:
	std::map<std::string, std::unique_ptr<Sound>> _sounds;
	sf::Music _music;
};
