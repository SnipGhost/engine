//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
bool Event::isKeyboardKey(sf::Keyboard::Key keyboard)
{
	if((type == sf::Event::KeyPressed) && (key.code == keyboard))
		return 1;
	return 0; 
}
//-----------------------------------------------------------------------------
bool Event::isWinClosed()
{
	if(type == sf::Event::Closed) return 1;
	return 0;
}
//-----------------------------------------------------------------------------
bool Event::isMouseKey(sf::Mouse::Button mouse)
{
	if(sf::Mouse::isButtonPressed(mouse)) return 1;
	return 0;
}
//-----------------------------------------------------------------------------
bool Event::isMusicPlay(sf::Music &music)
{
	if(music.getStatus() == sf::Music::Playing) return 1;
	return 0;
}
//-----------------------------------------------------------------------------