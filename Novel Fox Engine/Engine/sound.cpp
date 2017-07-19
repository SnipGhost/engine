//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sound.cpp                                           Реализация класса звуков
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sound::Sound(std::string _id, std::string src, float volume)
{
	setSound(src, volume);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sound::Sound(ResData rd)
{
	playable = rd.visible;
	setSound(rd.src, rd.volume);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sound::~Sound() // TODO: проверить [!]
{
	kernel.print("SOUND DELETED");
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
	os << "\tLooping:     \t" << ((s->getLoop()) ? "true" : "false") << std::endl;
	return os;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~