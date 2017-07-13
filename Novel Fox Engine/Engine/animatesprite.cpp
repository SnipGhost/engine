//-----------------------------------------------------------------------------
// animatesprite.cpp                   ���������� ������ �������������� �������
//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
AnimateSprite::AnimateSprite(std::string id, std::string src, bool smooth): Sprite(id, src, smooth)
{
	lastTime = 0;
	numFrame = 1;
	setResize(); //[!]
}
//-----------------------------------------------------------------------------
AnimateSprite::AnimateSprite(ResData rd) : Sprite(rd)
{
	lastTime = 0;
	numFrame = 1;
	setAnimation(rd.height, rd.width, rd.ms);
	setPosition(rd.x, rd.y);
	setScale(rd.scale, rd.scale);
	setResize(); //[!]
	id = rd.id;
	layer = rd.layer;
}
//-----------------------------------------------------------------------------
void AnimateSprite::setAnimation(int frameHeight, int frameWidth, int ms) 
{
	sideHeight = frameHeight;
	if (frameWidth == 0) frameWidth = frameHeight;
	sideWidth = frameWidth;
	delay = ms;
}
//-----------------------------------------------------------------------------
void AnimateSprite::setResize()
{
	float x = 1280;
	float y = 720;
	float k = x / y;

	if (WS_X*(1 / k) <= WS_Y)
	{
		setPosition(getPosition().x*KWS_X, getPosition().y*KWS_X + (WS_Y - WS_X * (1.0 / k)) / 2);
		setScale(getScale().x*KWS_X, getScale().y*KWS_X);
	}
	else
	{
		setPosition(getPosition().x*KWS_Y + ((WS_X - WS_Y * k) / 2), getPosition().y*KWS_Y);
		setScale(getScale().x*KWS_Y, getScale().y*KWS_Y);
	}
}
//-----------------------------------------------------------------------------
void AnimateSprite::update() 
{
	if ((kernel.globalClock.getMilliSecond()-lastTime) >= delay)
	{
		lastTime = kernel.globalClock.getMilliSecond();
		numFrame++;
		if (numFrame * sideWidth >= this->getTexture()->getSize().x)
			numFrame = 0;
	}
	setTextureRect(sf::IntRect(sideWidth*numFrame, 0, sideWidth, sideHeight));
}
//-----------------------------------------------------------------------------
void AnimateSprite::display(sf::RenderWindow *win) 
{
	this->update();
	win->draw(*this);
}
//-----------------------------------------------------------------------------
std::ostream &ng::operator << (std::ostream& os, const AnimateSprite &s)
{
	sf::Vector2f pos = s.getPosition();
	sf::Vector2f scl = s.getScale();
	os << s.id << " [ng::AnimateSprite]" << std::endl;
	os << "\tLayer:   \t" << s.layer << std::endl;
	os << "\tPosition:\t(" << pos.x << "; " << pos.y << ")" << std::endl;
	os << "\tScale:   \t(" << scl.x << "; " << scl.y << ")" << std::endl;
	return os;
}
//-----------------------------------------------------------------------------