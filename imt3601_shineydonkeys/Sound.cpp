#include "Sound.h"
#include "Logger.h"
#include "ConfigIO.h"

Sound::Sound(const std::string& filePath)
{
	if (!_buffer.loadFromFile(filePath))
		LOG_E("Failed to load WEAPON_SLASH sound");

	_sound.setBuffer(_buffer);
	_sound.setVolume(ConfigIO::readInt(L"audio", L"soundVolume", 1));
}

void Sound::play()
{
	_sound.play();
}
