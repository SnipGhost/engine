//-----------------------------------------------------------------------------
// sprite.cpp                                         Ðåàëèçàöèÿ êëàññà ñïàðéòà
//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Sprite::Sprite(std::string id, std::string src, bool smooth)
{
	if (!setStrTexture(src, smooth))
		kernel.print("Failed to create sprite «" + id + "»", WARN);
	else
		kernel.print("Sprite created «" + id + "»", INFO);
}
//-----------------------------------------------------------------------------
Sprite::Sprite(ResData rd)
{
	if (!setStrTexture(rd.src, rd.smooth))
		kernel.print("Failed to create sprite «" + rd.id + "»", WARN);
	else
		kernel.print("Sprite created «" + rd.id + "»", INFO);
	setPosition(rd.x, rd.y);
	setScale(rd.scale, rd.scale);
	id = rd.id;
	layer = rd.layer;
	// setResize();
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
// void Sprite::setResize()
// {
// 	float x = 1280;
// 	float y = 720;
// 	float k = x / y;

// 	if (WS_X*(1/k) <= WS_Y)
// 	{
// 		setPosition(getPosition().x*KWS_X, getPosition().y*KWS_X + (WS_Y - WS_X * (1.0 / k)) / 2);
// 		setScale(getScale().x*KWS_X, getScale().y*KWS_X);
// 	}
// 	else
// 	{
// 		setPosition(getPosition().x*KWS_Y + ((WS_X - WS_Y * k) / 2), getPosition().y*KWS_Y);
// 		setScale(getScale().x*KWS_Y, getScale().y*KWS_Y);
// 	}
// }
//-----------------------------------------------------------------------------
void Sprite::change(ResData rd)
{
	if (!setStrTexture(rd.src, rd.smooth))
		kernel.print("Failed to change sprite «" + rd.id + "»", WARN);
	else
		kernel.print("Sprite changed: «" + rd.id + "»", INFO);
	setPosition(rd.x, rd.y);
	setScale(rd.scale, rd.scale);
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
	os << "\tLayer:   \t" << s.layer << std::endl;
	os << "\tPosition:\t(" << pos.x << "; " << pos.y << ")" << std::endl;
	os << "\tScale:   \t(" << scl.x << "; " << scl.y << ")" << std::endl;
	return os;
}
//-----------------------------------------------------------------------------