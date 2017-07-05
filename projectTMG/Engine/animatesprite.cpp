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
void AnimateSprite::setAnimation(int frameHeight, int frameWidth, int ms) 
{
	sideHeight = frameHeight;
	if(frameWidth == 0) frameWidth = frameHeight; //Один аргумент setAnimation
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
void AnimateSprite::draw(sf::RenderWindow *win) 
{
	this->update();
	win->draw(*this);
}
//-----------------------------------------------------------------------------