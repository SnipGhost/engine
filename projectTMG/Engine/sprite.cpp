//-----------------------------------------------------------------------------
// sprite.cpp                                         Реализация класса спарйта
//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Sprite::Sprite(std::string src, bool smooth)
{
	if (!setStrTexture(src, smooth))
		kernel.print("ErrorSprite " + src, NORM);
}
//-----------------------------------------------------------------------------
Sprite::Sprite(SpriteData sd)
{
	if (!setStrTexture(sd.src, sd.smooth))
		kernel.print("ErrorSprite " + sd.src, NORM);
	setPosition(sd.x, sd.y);
	setScale(sd.scale, sd.scale);
}
//-----------------------------------------------------------------------------
bool Sprite::setStrTexture(std::string src, bool smooth)
{
	if (!texture.loadFromFile(src)) 
		return 0;
	texture.setSmooth(smooth);
	setTexture(texture);
	return 1;
}
//-----------------------------------------------------------------------------
void Sprite::change(SpriteData sd) 
{
	if (!setStrTexture(sd.src, sd.smooth))
		kernel.print("ErrorChangeSprite " + sd.src, NORM);
	setPosition(sd.x, sd.y);
	setScale(sd.scale, sd.scale);
}
//-----------------------------------------------------------------------------
void Sprite::draw(sf::RenderWindow *win) 
{
	win->draw(*this);
}
//-----------------------------------------------------------------------------