//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
//bool AnimateSprite::setStrTexture(std::string src, bool smooth)
//{
//	if (!texture.loadFromFile(src)) return 0;
//	texture.setSmooth(smooth);
//	setTexture(texture);
//	return 1;
//}
//-----------------------------------------------------------------------------
void AnimateSprite::setAnimation(int frameWidth, int frameHeight, int ms) 
{
	sideWidth = frameWidth;
	sideHeight = frameHeight;
	delay = ms;
}
//-----------------------------------------------------------------------------
void AnimateSprite::update() 
{
	    if ((ng::globalClock.getMilliSecond()-lastTime) >= delay)
		{
			lastTime = ng::globalClock.getMilliSecond();
			numFrame++;
			if (numFrame * sideWidth >= this->getTexture()->getSize().x)
			{
				numFrame = 0;
			}
		}
		setTextureRect(sf::IntRect(sideWidth * numFrame, 0, sideWidth, sideHeight));
}
//-----------------------------------------------------------------------------