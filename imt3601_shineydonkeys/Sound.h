#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

class Sound
{
public:
	explicit Sound(const std::string& filePath);

	void play();
	
	sf::SoundBuffer& getBuffer() { return _buffer; }
	void setBuffer(const sf::SoundBuffer& buffer) { _buffer = buffer; }

	sf::Sound& getSound() { return _sound; }
	void setSound(const sf::Sound& sound) { _sound = sound; }
private:
	sf::SoundBuffer _buffer;
	sf::Sound _sound;
};

