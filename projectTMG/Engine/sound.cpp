//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Sound::Sound(std::string src, float volume)
{
	if (!setSound(src, volume))
		kernel.print("ErrorSound " + src, NORM);
}
//-----------------------------------------------------------------------------
Sound::Sound(SoundData sod)
{
	if (!setSound(sod.src, sod.volume))
		kernel.print("ErrorSound " + sod.src, NORM);
}
//-----------------------------------------------------------------------------
bool Sound::setSound(std::string src, float volume)
{
	if (!buffer.loadFromFile(src)) 
		return 0;
	setBuffer(buffer);
	setVolume(volume);
	return 1;
}
//-----------------------------------------------------------------------------