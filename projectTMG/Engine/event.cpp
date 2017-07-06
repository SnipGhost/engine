//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
bool CheckEvent::isKeyboardKey(sf::Keyboard::Key keyboard, sf::Event event)
{
	if((event.type == sf::Event::KeyPressed) && (event.key.code == keyboard)) return 1;
	return 0; 
}
//-----------------------------------------------------------------------------
bool CheckEvent::isWinClosed(sf::Event event)
{
	if(event.type == sf::Event::Closed) return 1;
	return 0;
}
//-----------------------------------------------------------------------------
bool CheckEvent::isMouseKey(sf::Mouse::Button mouse)
{
	if(sf::Mouse::isButtonPressed(mouse)) return 1;
	return 0;
}
//-----------------------------------------------------------------------------
bool CheckEvent::isMusicPlay(sf::Music &music)
{
	if(music.getStatus() == sf::Music::Playing) return 1;
	return 0;
}
//-----------------------------------------------------------------------------