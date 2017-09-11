//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// music.cpp                                           Реализация класса музыки
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Music::Music(ResData rd)
{
	state = rd.command;
	playable = rd.visible;
	volume = rd.volume;
	volumeNow = rd.volume;
	loop = rd.loop;
	if (!setMusic(rd.src))
		kernel.print("Failed load music " + rd.id, WARN);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Music::setMusic(std::string src)
{
	if (!openFromFile(src)) 
		return 0;
	setVolume(volumeNow);
	//setPitch(2); // Установка скорости
	setLoop(loop);
	return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Music::update()
{
	//std::cout << state << std::endl;
	if (state == "final")
	{
		return;
	}
	if (state == "NULL")
	{
		state = "final";
	}
	if (state == "pause" && getStatus() == sf::Music::Playing)
	{
		state = "final";
		playable = false;
		pause();
		return;
	} else if (state == "pause" && getStatus() == sf::Music::Paused)
	{
		state = "final";
	}
	if (state == "play" && (getStatus() == sf::Music::Paused || getStatus() == sf::Music::Stopped))
	{
		state = "final";
		playable = true;
		volumeNow = volume;
		setVolume(volumeNow);
		play();
		return;
	}
	else if (state == "play" && getStatus() == sf::Music::Playing)
	{
		state = "final";
	}
	//if (state == "smoothpause")
	//{
	//	static float firstTime = kernel.globalClock.getMilliSecond();
	//	float nextTime = kernel.globalClock.getMilliSecond();
	//	float k = (nextTime - firstTime) / 100;

	//	if (getStatus() == sf::Music::Playing && k <= 100)
	//	{
	//		volumeNow = getVolume() - getVolume() * k / 100;

	//		if (volumeNow <= volume && volumeNow > 0.01)
	//		{
	//			std::cout << volumeNow << std::endl;
	//			setVolume(volumeNow);
	//		} 
	//		else
	//		{
	//			playable = false;
	//			pause();
	//			state = "final";
	//		}
	//	}
	//}
	//if (state == "smoothstop")
	//{
	//	// ?
	//}
	//if (state == "smoothplay")
	//{
	//	playable = true;
	//	play();

	//	static float firstTime = kernel.globalClock.getMilliSecond();
	//	float nextTime = kernel.globalClock.getMilliSecond();
	//	float k = (nextTime - firstTime) / 100;

	//	if (getStatus() == sf::Music::Playing && k <= 100)
	//	{
	//		volumeNow = getVolume() + getVolume() * k / 100;

	//		if (volumeNow <= volume && volumeNow > 0.01)
	//		{
	//			setVolume(volumeNow);
	//		}
	//		else
	//		{
	//			state = "final";
	//		}
	//	}
	//}
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