#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Music::Music(std::string id, std::string src, float volume, bool loop)
{
	this->id = id; //TO DO
	if (!setMusic(src, volume, loop))
	std::cout << "ErrorMusic " << src << std::endl;
}
//-----------------------------------------------------------------------------
//Music::Music(MusicData md)
//{
//	this->id = md.id; //TO DO
//	if (!setMusic(md.src, md.volume, md.loop))
//	std::cout << "ErrorMusic " << md.src << std::endl;
//}
//-----------------------------------------------------------------------------
bool Music::setMusic(std::string src, float volume, bool loop)
{
	if (!openFromFile(src)) return 0;
	setVolume(volume);
	setLoop(loop);
	return 1;
}
//-----------------------------------------------------------------------------
//void Music::change() 
//{
//
//}
//-----------------------------------------------------------------------------