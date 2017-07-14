//-----------------------------------------------------------------------------
// animatesprite.cpp                   Реализация класса анимированного спарйта
//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
AnimateSprite::AnimateSprite(std::string id, std::string src, bool smooth): Sprite(id, src, smooth)
{
	lastTime = 0;
	numFrame = 1;
}
//-----------------------------------------------------------------------------
AnimateSprite::AnimateSprite(ResData rd) : Sprite(rd)
{
	lastTime = 0;
	numFrame = 1;
	setAnimation(rd.height, rd.width, rd.ms);
	setPosition(rd.x, rd.y);
	setScale(rd.scale, rd.scale);
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
std::ostream &ng::operator << (std::ostream& os, AnimateSprite &s)
{
	return s.print(os);
}
//-----------------------------------------------------------------------------