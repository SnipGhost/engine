//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// music.cpp                                           Реализация класса музыки
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Music::Music(ResData rd)
{
	state = "normal";
	playable = rd.visible;
	if (!setMusic(rd.src, rd.volume, rd.loop))
		kernel.print("Failed load music " + rd.id, WARN);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Music::setMusic(std::string src, float volume, bool loop)
{
	if (!openFromFile(src)) 
		return 0;
	setVolume(volume);
	//setPitch(2); // Установка скорости
	setLoop(loop);
	return 1;
}
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//bool Music::setSlowStop()
//{
//	static float firstTime = kernel.globalClock.getMilliSecond();
//	float nextTime = kernel.globalClock.getMilliSecond();
//	float k = (nextTime - firstTime)/10;
//
//	std::cout << firstTime << std::endl;
//
//	if (getStatus() == sf::Music::Playing && k <= 100) {
//		volume = getVolume() - getVolume()*k/100;
//
//		if (volume <= 100 && volume >= 0) setVolume(volume);
//		return 0; 
//	}
//	else
//	{
//		return 1;
//	}
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Music::setPause()
{
	if (getStatus() == sf::Music::Playing)
		pause();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//void Music::change() 
//{
//
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::ostream &ng::operator << (std::ostream &os, const Music *m)
{
	os << "[ng::Music]" << std::endl;
	os << "\tDuration (sec):\t" << m->getDuration().asSeconds() << std::endl;
	os << "\tChannel count: \t" << m->getChannelCount() << std::endl;
	os << "\tVolume level:  \t" << m->getVolume() << std::endl;
	os << "\tLooping:     \t" << ((m->getLoop()) ? "true" : "false") << std::endl;
	return os;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~