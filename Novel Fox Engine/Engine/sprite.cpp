//-----------------------------------------------------------------------------
// sprite.cpp                                         Реализация класса спарйта
//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Sprite::Sprite(std::string id, std::string src, bool smooth)
{
	if (!setStrTexture(src, smooth))
		kernel.print("Failed to create sprite " + id, WARN);
	else
		kernel.print("Sprite created " + id, INFO);
}
//-----------------------------------------------------------------------------
Sprite::Sprite(ResData rd)
{
	if (!setStrTexture(rd.src, rd.smooth))
		kernel.print("Failed to create sprite " + rd.id, WARN);
	else
		kernel.print("Sprite created " + rd.id, INFO);
	setPosition(rd.x, rd.y);
	setScale(rd.scale, rd.scale);
	id = rd.id;
	layer = rd.layer;
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
void Sprite::change(ResData rd)
{
	if (!setStrTexture(rd.src, rd.smooth))
		kernel.print("Failed to change sprite " + rd.id, WARN);
	else
		kernel.print("Sprite changed " + rd.id, INFO);
	setPosition(rd.x, rd.y);
	setScale(rd.scale, rd.scale);
}
//-----------------------------------------------------------------------------
void Sprite::display(sf::RenderWindow *win) 
{
	win->draw(*this);
}
//-----------------------------------------------------------------------------
std::ostream &ng::operator << (std::ostream& os, Sprite &s)
{
	return s.print(os);
}
//-----------------------------------------------------------------------------