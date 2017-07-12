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
Sound::Sound(ResData rd)
{
	if (!setSound(rd.src, rd.volume))
		kernel.print("Failed load sound " + rd.src, WARN);
	else
		kernel.print("Load sound " + rd.src, INFO);
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
std::ostream &ng::operator << (std::ostream &os, const Sound &s)
{
	os << "[ng::Sound]" << std::endl;
	os << "\tVolume level: \t" << s.getVolume() << std::endl;
	os << "\tLooping value:\t" << s.getLoop() << std::endl;
	return os;
}
//-----------------------------------------------------------------------------