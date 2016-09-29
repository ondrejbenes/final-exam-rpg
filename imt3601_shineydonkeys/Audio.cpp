#include "Audio.h"


Audio::Audio() : Module(AUDIO)
{

}



Audio::~Audio(){

}

bool Audio::initialize()
{

	return true;
}


void Audio::playSound(sf::SoundBuffer soundName){

}


void Audio::playTrack(sf::SoundBuffer trackName){

}