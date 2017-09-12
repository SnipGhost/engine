//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// music.cpp                                           Реализация класса музыки
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Music::Music(ResData rd)
{
	first = true;			// Логическая переменная для различных действий
	tact = 100;				// Частота изменения [100 - 1 раз в 100 миллисекунд]
	timeDo = rd.time;		// Время изменения
	state = rd.command;		// Текущее состояние
	playable = rd.visible;
	volume = rd.volume;		// Установленная громкость музыки
	volumeNow = rd.volume;	// Громкость музыки в текущий момент времени
	loop = rd.loop;			// Повторение музыки
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
		pause();
		return;
	} else if (state == "pause" && getStatus() == sf::Music::Paused)
	{
		state = "final";
	}
	if (state == "play" && (getStatus() == sf::Music::Paused || getStatus() == sf::Music::Stopped))
	{
		state = "final";
		volumeNow = volume;
		setVolume(volumeNow);
		play();
		return;
	}
	else if (state == "play" && getStatus() == sf::Music::Playing)
	{
		state = "final";
	}

	if (state == "smoothpause")
	{
		if (first)
		{
			firstTime = kernel.globalClock.getMilliSecond();
			first = false;
		}
		nextTime = kernel.globalClock.getMilliSecond();
		difference = nextTime - firstTime;
		if (getStatus() == sf::Music::Playing && difference >= tact)
		{
			firstTime = kernel.globalClock.getMilliSecond();
			volumeNow = getVolume() - (volume / (timeDo / tact));
			if (volumeNow < 0) volumeNow = 0;

			std::cout << volumeNow << std::endl;

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

	if (state == "smoothplay")
	{
		if (getStatus() == sf::Music::Stopped || getStatus() == sf::Music::Paused)
		{
			volumeNow = 0; // Начинаем с нуля volume
			setVolume(volumeNow);
			playable = true;
			play();
		}
		if (first)
		{
			firstTime = kernel.globalClock.getMilliSecond();
			first = false;
		}
		nextTime = kernel.globalClock.getMilliSecond();
		difference = nextTime - firstTime;
		if (getStatus() == sf::Music::Playing && difference >= tact)
		{
			firstTime = kernel.globalClock.getMilliSecond();
			volumeNow = getVolume() + (volume / (timeDo / tact));
			if (volumeNow > volume) volumeNow = volume;

			std::cout << volumeNow << std::endl;

			setVolume(volumeNow);
			if (volumeNow == volume)
			{
				state = "final";
				return;
			}
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Music::edit(ResData rd)
{
	if (GETBIT(rd.bitMask, _command))
	{
		if (rd.command == "pause")
		{
			playable = false;
			state = rd.command;
		}
		if (rd.command == "play")
		{
			playable = true;
			state = rd.command;
		}

		if (rd.command == "smoothpause" || rd.command == "smoothplay")
		{
			state = rd.command;
		}
	}

	if (GETBIT(rd.bitMask, _time)) timeDo = rd.time;
	if (GETBIT(rd.bitMask, _volume)) // Продумать над плавностью [!]
	{
		volume = rd.volume;
		volumeNow = rd.volume;
		setVolume(volumeNow);
	}
	if (GETBIT(rd.bitMask, _loop))
	{
		loop = rd.loop;
		setLoop(loop);
	}
	if (GETBIT(rd.bitMask, _src)) setMusic(rd.src); // Добавил как возможность, но это отвратно
}
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