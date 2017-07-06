//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Sound::Sound(std::string src, float volume)
{
	if (!setSound(src, volume))
		kernel.print("Failed load sound " + src, WARN);
	else
		kernel.print("Created sound " + src, INFO);
}
//-----------------------------------------------------------------------------
Sound::Sound(SoundData sod)
{
	if (!setSound(sod.src, sod.volume))
		kernel.print("Failed load sound " + sod.src, WARN);
	else
		kernel.print("Load sound " + sod.src, INFO);
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