//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// music.cpp                                           Реализация класса музыки
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Music::Music(ResData rd)
{
	first = true;

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

	if (state == "smoothpause") // Кривая версия [!]
	{
		if (first)
		{
			firstTime = kernel.globalClock.getMilliSecond();
			first = false;
		}
		nextTime = kernel.globalClock.getMilliSecond();
		
		float k = (nextTime - firstTime);
		float tact = 90; // 90 миллисекунд [!] Непонятная константа
		timeDo = 3; // 3 секунды
		std::cout << getVolume() << std::endl;
		if (getStatus() == sf::Music::Playing && k >= tact)
		{
			firstTime = kernel.globalClock.getMilliSecond();

			volumeNow = getVolume() - (1000 / timeDo / volume);
			if (volumeNow < 0) volumeNow = 0;
			setVolume(volumeNow);

			if (volumeNow == 0)
			{
				state = "final";
				playable = false;
				pause();
				first = true;
				return;
			}
		}
	}

	if (state == "smoothplay") // Кривая версия [!]
	{
		if (getStatus() == sf::Music::Stopped)
		{
			volumeNow = 0; // Начинаем с нуля volume
			setVolume(volumeNow);
			playable = true;
			play();
		}
			
		if (getStatus() == sf::Music::Paused)
		{
			playable = true;
			play();
		}

		if (first)
		{
			firstTime = kernel.globalClock.getMilliSecond();
			first = false;
		}
		nextTime = kernel.globalClock.getMilliSecond();

		float k = (nextTime - firstTime);
		float tact = 90; // 90 миллисекунд [!] Непонятная константа
		timeDo = 3; // 3 секунды

		if (getStatus() == sf::Music::Playing && k >= tact)
		{
			firstTime = kernel.globalClock.getMilliSecond();

			volumeNow = getVolume() + (1000 / timeDo / volume);
			if (volumeNow > volume) volumeNow = volume;
			setVolume(volumeNow);
			std::cout << getVolume() << std::endl;

			if (volumeNow == volume)
			{
				state = "final";
				return;
			}
		}
	}
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