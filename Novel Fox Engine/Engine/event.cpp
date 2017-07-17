//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// event.cpp                                          Реализация класса событий
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ЕСЛИ НАЖАТА КЛАВИША]
bool Event::isKeyboardKey(sf::Keyboard::Key keyboard)
{
	static bool flag = true;
	if(flag == true && type == KeyPressed && key.code == keyboard) 
	{
			flag = false;
			return true;
	}

	if(type == KeyReleased && key.code == keyboard) 
		flag = true;

	return false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ЕСЛИ ОКНО ЗАКРЫТО]
bool Event::isWinClosed()
{
	if(type == sf::Event::Closed) 
		return true;
	return false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ЕСЛИ ПРОИЗВЕЛИ КЛИК КНОПКИ МЫШИ]
bool Event::isMouseClickKey(sf::Mouse::Button mouse)
{
	static bool flag = true;
	if (flag == true && type == MouseButtonPressed && mouseButton.button == mouse) 
	{
		flag = false;
		std::string m = "Mouse click: (" + std::to_string(mouseButton.x);
		m += "; " + std::to_string(mouseButton.y) + ")";
		kernel.print(m);
		return true;
	}

	if (type == MouseButtonReleased && mouseButton.button == mouse) 
		flag = true;

	return false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ЕСЛИ НАЖАТА КНОПКА МЫШИ]
bool Event::isMouseKey(sf::Mouse::Button mouse)
{
	if(type == MouseButtonPressed && mouseButton.button == mouse) 
		return true;
	return false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ЕСЛИ ДАННАЯ МУЗЫКА ИГРАЕТ]
bool Event::isMusicPlay(sf::Music &music)
{
	if(music.getStatus() == sf::Music::Playing) 
		return true;
	return false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ЕСЛИ ДАННОЕ ВИДЕО ПРОИГРЫВАЕТСЯ]
bool Event::isVideoPlay(sfe::Movie &video)
{
	if (video.getStatus() == sfe::Playing)
		return true;
	return false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~