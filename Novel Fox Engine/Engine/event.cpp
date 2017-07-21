//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// event.cpp                                          Реализация класса событий
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Если нажата клавиша
bool Event::isKeyboardKey(sf::Keyboard::Key keyboard)
{
	static bool flag = true;
	if (flag && type == KeyPressed && key.code == keyboard) 
	{
		flag = false;
		return true;
	}

	if (type == KeyReleased && key.code == keyboard) 
		flag = true;

	return false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Если окно закрыто
bool Event::isWinClosed()
{
	return (type == sf::Event::Closed);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Если произвели клик кнопки мыши
bool Event::isMouseClickKey(sf::Mouse::Button mouse)
{
	static bool flag = true;
	if (flag && type == MouseButtonPressed && mouseButton.button == mouse) 
	{
		flag = false;

		//Регистрация клика [УБРАТЬ]
		kernel.print("Mouse click: (" + std::to_string(mouseButton.x) + "; " 
			+ std::to_string(mouseButton.y) + ")");

		return true;
	}

	if (type == MouseButtonReleased && mouseButton.button == mouse) 
		flag = true;

	return false;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Если нажата кнопка мыши
bool Event::isMouseKey(sf::Mouse::Button mouse)
{
	return (type == MouseButtonPressed && mouseButton.button == mouse);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Если данная музыка играет
bool Event::isMusicPlay(sf::Music &music)
{
	return (music.getStatus() == sf::Music::Playing);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Если данное видео проигрывается
bool Event::isVideoPlay(sfe::Movie &video)
{
	return (video.getStatus() == sfe::Playing);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~