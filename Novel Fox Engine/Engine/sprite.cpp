//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sprite.cpp                                         Реализация класса спрайта
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sprite::Sprite(std::string id, std::string src, bool smooth)
{
	if (!setStrTexture(src, smooth))
		kernel.print("Failed to create sprite " + id, WARN);
	else
		kernel.print("Sprite created " + id, INFO);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sprite::Sprite(ResData rd)
{
	if (!setStrTexture(rd.src, rd.smooth))
		kernel.print("Failed to create sprite " + rd.id, WARN);
	else
		kernel.print("Sprite created " + rd.id, INFO);

	setColor(sf::Color(255, 255, 255, rd.alpha));
	
	id = rd.id;
	layer = rd.layer;
	visible = rd.visible;

	origin = PosScale(rd.x, rd.y, rd.scale, rd.scale);
	setResize();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Sprite::setStrTexture(std::string src, bool smooth)
{
	if (!texture.loadFromFile(src)) 
		return 0;
	texture.setSmooth(smooth);
	setTexture(texture);
	return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//void Sprite::change(ResData rd)
//{
//	if (!setStrTexture(rd.src, rd.smooth))
//		kernel.print("Failed to change sprite " + rd.id, WARN);
//	else
//		kernel.print("Sprite changed " + rd.id, INFO);
//	setPosition(rd.x, rd.y);
//	setScale(rd.scale, rd.scale);
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Sprite::display(sf::RenderWindow *win) 
{
	win->draw(*this);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::ostream &ng::operator << (std::ostream& os, Sprite &s)
{
	return s.print(os);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::ostream & Sprite::print(std::ostream &os)
{
	sf::Vector2f pos = getPosition();
	sf::Vector2f scl = getScale();
	os << id << " [ng::Sprite]" << std::endl;
	os << "\tLayer:   \t" << layer << std::endl;
	os << "\tPosition:\t(" << pos.x << "; " << pos.y << ")" << std::endl;
	os << "\tScale:   \t(" << scl.x << "; " << scl.y << ")" << std::endl;
	os << "\tReSize:  \t(" << KWS_X << "; " << KWS_Y << ")" << std::endl;
	return os;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Sprite::setResize() 
{ 
	Displayable::setResize();
	computeLayerScale();
	setPosition(posScale.pos);
	setScale(posScale.scale);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Sprite::setLayerMotion() 
{ 
	doLayerMotion(this); 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Sprite::computeLayerScale()
{
	int w = this->getTextureRect().width;
	int h = this->getTextureRect().height;

	float sx = posScale.scale.x + (float)0.03 * (2 << (layer - 1)) * KWS_X;
	float sy = posScale.scale.y + (float)0.03 * (2 << (layer - 1)) * KWS_X;

	posScale.pos.x = posScale.pos.x - w * (sx - posScale.scale.x) / 2;
	posScale.pos.y = posScale.pos.y - h * (sy - posScale.scale.y) / 2;

	posScale.scale.x = sx;
	posScale.scale.y = sy;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~