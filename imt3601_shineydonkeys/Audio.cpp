#include "Audio.h"
#include "Blackboard.h"
#include "Logger.h"
#include "ConfigIO.h"

Audio::Audio() : Module(AUDIO)
{

}

bool Audio::initialize()
{
	reloadVolumeFromConfig();

	return true;
}

void Audio::update() 
{
	auto callbacks = Blackboard::getInstance()->getCallbacks(moduleType);
	for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
		(*it)(this);
}

void Audio::playSound(const std::string& soundName)
{
	if(_sounds.find(soundName) == end(_sounds))
		_sounds[soundName] = std::make_unique<Sound>(soundName);

	_sounds[soundName]->play();
}

void Audio::playMusic(const std::string& musicName, bool loop)
{
	if(!_music.openFromFile(musicName))
		LOG_E("Failed to open music from file");

	_music.setLoop(loop);
	_music.play();
}

void Audio::reloadVolumeFromConfig() 
{
	auto soundVolume = ConfigIO::readInt(L"audio", L"soundVolume", 1);
	auto musicVolume = ConfigIO::readInt(L"audio", L"musicVolume", 1);

	for (auto& s : _sounds)
		s.second->getSound().setVolume(soundVolume);

	_music.setVolume(musicVolume);
}

const std::string Audio::WEAPON_SLASH = "Resources/Audio/weapon_slash.wav";
const std::string Audio::HUMAN_NPC_DYING = "Resources/Audio/human_npc_dying.wav";

const std::string Audio::THEME_SONG = "Resources/Audio/marchingbyriver_theme.ogg";