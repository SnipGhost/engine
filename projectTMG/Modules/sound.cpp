#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Sound::Sound(std::string id, std::string src, float volume)
{
	this->id = id; //TO DO
	if (!setSound(src, volume))
	std::cout << "ErrorSound " << src << std::endl;
}
//-----------------------------------------------------------------------------
//Sound::Sound(SoundData sod)
//{
//	this->id = sod.id; //TO DO
//	if (!setSound(sod.src, sod.volume))
//	std::cout << "ErrorSound " << sod.src << std::endl;
//}
//-----------------------------------------------------------------------------
bool Sound::setSound(std::string src, float volume)
{
	if (!buffer.loadFromFile(src)) return 0;
	setBuffer(buffer);
	setVolume(volume);
	return 1;
}
//-----------------------------------------------------------------------------
//void Music::change() 
//{
//
//}
//-----------------------------------------------------------------------------