//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// animatesprite.cpp                   Реализация класса анимированного спрайта
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AnimateSprite::AnimateSprite(std::string id, std::string src, bool smooth): Sprite(id, src, smooth)
{
	lastTime = 0;
	numFrame = 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AnimateSprite::AnimateSprite(ResData rd) : Sprite(rd)
{
	lastTime = 0;
	numFrame = 1;
	setAnimation(rd.height, rd.width, rd.delay);
	
	origin = PosScale(rd.x, rd.y, rd.scale, rd.scale);
	setResize();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AnimateSprite::setAnimation(int frameHeight, int frameWidth, int ms) 
{
	sideHeight = frameHeight;
	if (frameWidth == 0) frameWidth = frameHeight;
	else sideWidth = frameWidth;
	delay = ms;
	setTextureRect(sf::IntRect(sideWidth*numFrame, 0, sideWidth, sideHeight));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AnimateSprite::edit(ResData rd)
{
	//if (GETBIT(rd.bitMask, _alpha)) setColor(sf::Color(255, 255, 255, rd.alpha));
	if (GETBIT(rd.bitMask, _x) && GETBIT(rd.bitMask, _y)) //ЕСЛИ ЕСТЬ ДВЕ КООРДИНАТЫ - ФИКСИТЬ
	{
		positionObj = sf::Vector2f(rd.x, rd.y); //Перезаписали стандартные значения
		setPosition(rd.x, rd.y);
		origin = PosScale(rd.x, rd.y, getScaleObj(), getScaleObj());
		setResize();
	}
	if (GETBIT(rd.bitMask, _scale))
	{
		scaleObj = rd.scale; //Перезаписали стандартные значения
		setScale(rd.scale, rd.scale);
		origin = PosScale(getPositionObj().x, getPositionObj().y, rd.scale, rd.scale);
		setResize();
	}
	if (GETBIT(rd.bitMask, _src)) setStrTexture(rd.src, true);
	if (GETBIT(rd.bitMask, _width) && GETBIT(rd.bitMask, _height))
	{
		sideHeight = rd.height;
		if (rd.width == 0) rd.width = rd.height;
		else sideWidth = rd.width;
		setTextureRect(sf::IntRect(sideWidth*numFrame, 0, sideWidth, sideHeight));
	}
	if (GETBIT(rd.bitMask, _delay)) delay = _delay;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AnimateSprite::display(sf::RenderWindow *win) 
{
	this->update();
	win->draw(*this);
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
	os << "\tReSize:  \t(" << kernel.factor.x << "; " << kernel.factor.y << ")" << std::endl;
	return os;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~