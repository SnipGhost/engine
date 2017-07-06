//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
bool Event::isKeyboardKey(sf::Keyboard::Key keyboard)
{
	if(type == KeyReleased && key.code == keyboard) return true;
	return false;
}
//-----------------------------------------------------------------------------
bool Event::isWinClosed()
{
	if(type == sf::Event::Closed) return true;
	return false;
}
//-----------------------------------------------------------------------------
bool Event::isMouseKey(sf::Mouse::Button mouse)
{
	if (type == MouseButtonReleased && mouseButton.button == mouse) return true;
	return false;
}
//-----------------------------------------------------------------------------
bool Event::isMusicPlay(sf::Music &music)
{
	if(music.getStatus() == sf::Music::Playing) return true;
	return false;
}
//-----------------------------------------------------------------------------