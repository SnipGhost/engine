//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sound.cpp                                           Реализация класса звуков
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sound::Sound(std::string id, std::string src, float _volume)
{
	volume = _volume;
	playable = true;
	if (!setSound(src, volume))
		kernel.print("Failed load sound " + id, WARN);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sound::Sound(ResData rd)
{
	volume = rd.volume;
	playable = rd.visible;
	if (!setSound(rd.src, rd.volume))
		kernel.print("Failed load sound " + rd.id, WARN);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Sound::edit(ResData rd)
{
	playable = true;
	play(); // Проиграет даже при Edit

	if (GETBIT(rd.bitMask, _src))
	{
		setSound(rd.src, volume);
	}
	if (GETBIT(rd.bitMask, _volume))
	{
		volume = rd.volume;
		setVolume(rd.volume);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Sound::setSound(std::string src, float volume)
{
	if (!buffer.loadFromFile(src)) 
		return 0;
	setBuffer(buffer);
	setVolume(volume);
	return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::ostream &ng::operator << (std::ostream &os, const Sound *s)
{
	os << "[ng::Sound]" << std::endl;
	os << "\tVolume level: \t" << s->getVolume() << std::endl;
	return os;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~