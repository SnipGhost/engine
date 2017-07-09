//-----------------------------------------------------------------------------
// animatesprite.cpp                   Реализация класса анимированного спарйта
//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
AnimateSprite::AnimateSprite(std::string src, bool smooth): Sprite(src, smooth)
{
	lastTime = 0;
	numFrame = 1;
}
//-----------------------------------------------------------------------------
AnimateSprite::AnimateSprite(AnimateSpriteData asd) : Sprite(asd.src, asd.smooth)
{
	lastTime = 0;
	numFrame = 1;
	setAnimation(asd.frameHeight, asd.frameWidth, asd.ms);
	setPosition(asd.x, asd.y);
	setScale(asd.scale, asd.scale);
	id = asd.id;
	layer = asd.layer;
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
	os << "\tLayer: " << s.layer << std::endl;
	os << "\tPosition: (" << pos.x << "; " << pos.y << ")" << std::endl;
	os << "\tScale: (" << scl.x << "; " << scl.y << ")" << std::endl;
	return os;
}
//-----------------------------------------------------------------------------