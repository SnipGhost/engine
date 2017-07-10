//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
bool Event::isWinClosed()
{
	if(type == sf::Event::Closed) 
		return true;
	return false;
}
//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
bool Event::isMouseKey(sf::Mouse::Button mouse)
{
	if(type == MouseButtonPressed && mouseButton.button == mouse) 
		return true;
	return false;
}
//-----------------------------------------------------------------------------
bool Event::isMusicPlay(sf::Music &music)
{
	if(music.getStatus() == sf::Music::Playing) 
		return true;
	return false;
}
//-----------------------------------------------------------------------------
bool Event::isVideoPlay(sfe::Movie &video)
{
	if (video.getStatus() == sfe::Playing)
		return true;
	return false;
}
//-----------------------------------------------------------------------------