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
		kernel.print("Failed load texture " + src, WARN);
	else
		kernel.print("Load texture " + src, INFO);
}
//-----------------------------------------------------------------------------
Sprite::Sprite(SpriteData sd)
{
	if (!setStrTexture(sd.src, sd.smooth))
		kernel.print("Failed create sprite " + sd.src, WARN);
	else
		kernel.print("Create sprite " + sd.src, INFO);
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
		kernel.print("Failed create sprite " + sd.src, WARN);
	else
		kernel.print("Change sprite " + sd.src, INFO);
	setPosition(sd.x, sd.y);
	setScale(sd.scale, sd.scale);
}
//-----------------------------------------------------------------------------
void Sprite::display(sf::RenderWindow *win) 
{
	win->draw(*this);
}
//-----------------------------------------------------------------------------