#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Music::Music(std::string src, float volume, bool loop)
{
	if (!setMusic(src, volume, loop))
		std::cout << "ErrorMusic " << src << std::endl;
}
//-----------------------------------------------------------------------------
Music::Music(MusicData md)
{
	if (!setMusic(md.src, md.volume, md.loop))
	std::cout << "ErrorMusic " << md.src << std::endl;
}
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