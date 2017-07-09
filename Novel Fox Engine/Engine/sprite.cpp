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
		kernel.print("Failed to load texture " + src, WARN);
	else
		kernel.print("Load texture " + src, INFO);
}
//-----------------------------------------------------------------------------
Sprite::Sprite(SpriteData sd)
{
	if (!setStrTexture(sd.src, sd.smooth))
		kernel.print("Failed to create sprite " + sd.id, WARN);
	else
		kernel.print("Sprite created " + sd.id, INFO);
	setPosition(sd.x, sd.y);
	setScale(sd.scale, sd.scale);
	id = sd.id;
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
		kernel.print("Failed to change sprite " + sd.id, WARN);
	else
		kernel.print("Sprite changed: " + sd.id, INFO);
	setPosition(sd.x, sd.y);
	setScale(sd.scale, sd.scale);
}
//-----------------------------------------------------------------------------
void Sprite::display(sf::RenderWindow *win) 
{
	win->draw(*this);
}
//-----------------------------------------------------------------------------
std::ostream &ng::operator << (std::ostream& os, const Sprite &s)
{
	sf::Vector2f pos = s.getPosition();
	sf::Vector2f scl = s.getScale();
	os << s.id << " [ng::Sprite]" << std::endl;
	os << "\tLayer: " << s.layer << std::endl;
	os << "\tPosition: (" << pos.x << "; " << pos.y << ")" << std::endl;
	os << "\tScale: (" << scl.x << "; " << scl.y << ")" << std::endl;
	return os;
}
//-----------------------------------------------------------------------------