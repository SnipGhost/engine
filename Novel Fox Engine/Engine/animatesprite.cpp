//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// animatesprite.cpp                   Реализация класса анимированного спрайта
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AnimateSprite::AnimateSprite(std::string id, std::string src, bool smooth): Sprite(id, src, smooth) // Не используется [!]
{
	lastTime = 0;

	xPozAnim = 0;
	yPozAnim = 0;

	xEnd = (getTexture()->getSize().x / 256) - 1;
	yEnd = (getTexture()->getSize().y / 256) - 1;

	setAnimation(256, 256, 40);

	origin = PosScale(0, 0, 1, 1);
	setResize();

	//Нужно ли делать? [!]
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AnimateSprite::AnimateSprite(ResData rd) : Sprite(rd)
{
	lastTime = 0;

	xPozAnim = 0;
	yPozAnim = 0;

	xEnd = (getTexture()->getSize().x / rd.width) - 1;
	yEnd = (getTexture()->getSize().y / rd.height) - 1;

	setAnimation(rd.height, rd.width, rd.delay);
	
	origin = PosScale(rd.x, rd.y, rd.scale, rd.scale);
	setResize();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AnimateSprite::setAnimation(int frameHeight, int frameWidth, int ms) 
{
	sideHeight = frameHeight;
	sideWidth = frameWidth;
	delay = ms;
	setTextureRect(sf::IntRect(0, 0, sideWidth, sideHeight)); // Без этого происходит странный баг свзяанный с первоначальным resize
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AnimateSprite::update() 
{
	if ((kernel.globalClock.getMilliSecond()-lastTime) >= delay)
	{
		lastTime = kernel.globalClock.getMilliSecond();
		if (xPozAnim != xEnd) // ОЧЕНЬ ЖЁСТКАЯ СИСТЕМА
		{
			xPozAnim++;
		}
		else
		{
			if (yPozAnim != yEnd)
			{
				yPozAnim++;
				xPozAnim = 0;
			}
			else
			{
				xPozAnim = 0;
				yPozAnim = 0;
			}
		}
	}
	setTextureRect(sf::IntRect(sideWidth * xPozAnim, sideHeight * yPozAnim, sideWidth, sideHeight));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool AnimateSprite::isMouseAbove()
{
	if (kernel.getMouse().x >= posScale.pos.x &&
		kernel.getMouse().x < (posScale.pos.x + getTextureRect().width * posScale.scale.x) &&
		kernel.getMouse().y >= posScale.pos.y &&
		kernel.getMouse().y < (posScale.pos.y + getTextureRect().height * posScale.scale.y))
	{
		return 1;
	}
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AnimateSprite::edit(ResData rd)
{
	if (GETBIT(rd.bitMask, _visible))
	{
		visible = rd.visible;
	}
	if (GETBIT(rd.bitMask, _alpha))
	{
		alpha = rd.alpha;
		setAlpha(rd.alpha);
	}
	if (GETBIT(rd.bitMask, _x) || GETBIT(rd.bitMask, _y))
	{
		if (GETBIT(rd.bitMask, _x))
		{
			positionObj.x = rd.x;
			setPosition(rd.x, positionObj.y);
			origin = PosScale(rd.x, positionObj.y, scaleObj, scaleObj);
		}
		if (GETBIT(rd.bitMask, _y))
		{
			positionObj.y = rd.y;
			setPosition(positionObj.x, rd.y);
			origin = PosScale(positionObj.x, rd.y, scaleObj, scaleObj);
		}
		setResize();
	}
	if (GETBIT(rd.bitMask, _scale))
	{
		scaleObj = rd.scale;
		setScale(rd.scale, rd.scale);
		origin = PosScale(positionObj.x, positionObj.y, rd.scale, rd.scale);
		setResize();
	}
	if (GETBIT(rd.bitMask, _src))
	{
		setStrTexture(rd.src, rd.smooth);
	}
	if (GETBIT(rd.bitMask, _width))
	{
		sideWidth = rd.width;
		xEnd = (getTexture()->getSize().x / rd.width) - 1;
		xPozAnim = 0;
	}
	if (GETBIT(rd.bitMask, _height))
	{
		sideHeight = rd.height;
		yEnd = (getTexture()->getSize().y / rd.height) - 1;
		yPozAnim = 0;
	}
	if (GETBIT(rd.bitMask, _delay))
	{
		delay = rd.delay;
	}
	if (GETBIT(rd.bitMask, _style))
	{
		style = rd.style;
		setRenderStates(rd.style);
	}
	if (GETBIT(rd.bitMask, _angle))
	{
		angle = rd.angle;
		setRotation(rd.angle);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AnimateSprite::display(sf::RenderWindow *win) 
{
	update();
	win->draw(*this, renderStates);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::ostream &ng::operator << (std::ostream& os, AnimateSprite &s)
{
	return s.print(os);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::ostream & AnimateSprite::print(std::ostream &os)
{
	sf::Vector2f pos = getPosition();
	sf::Vector2f scl = getScale();
	os << id << " [ng::AnimateSprite]" << std::endl;
	os << "\tLayer:   \t" << layer << std::endl;
	os << "\tPosition:\t(" << pos.x << "; " << pos.y << ")" << std::endl;
	os << "\tScale:   \t(" << scl.x << "; " << scl.y << ")" << std::endl;
	return os;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~