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
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sprite::Sprite(ResData rd)
{
	if (!setStrTexture(rd.src, rd.smooth))
		kernel.print("Failed to create sprite " + rd.id, WARN);

	setColor(sf::Color(255, 255, 255, rd.alpha));
	
	id = rd.id;
	layer = rd.layer;
	layermotion = rd.layermotion;
	visible = rd.visible;
	positionObj = sf::Vector2f(rd.x, rd.y);
	scaleObj = rd.scale;

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
void Sprite::edit(ResData rd)
{

	//Layer - НЕЛЬЗЯ
	//Smooth - НЕ РЕКОМЕНДУЕТСЯ
	if (GETBIT(rd.bitMask, _alpha)) setColor(sf::Color(255, 255, 255, rd.alpha));
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
	if (GETBIT(rd.bitMask, _src) && GETBIT(rd.bitMask, _smooth)) setStrTexture(rd.src, rd.smooth);
	kernel.print("Edit mode for sprite: " + rd.id, INFO);
}
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
	os << "\tLayerMotion:   \t" << ((layermotion) ? "true" : "false") << std::endl;
	os << "\tPosition:\t(" << pos.x << "; " << pos.y << ")" << std::endl;
	os << "\tScale:   \t(" << scl.x << "; " << scl.y << ")" << std::endl;
	os << "\tReSize:  \t(" << kernel.factor.x << "; " << kernel.factor.y << ")" << std::endl;
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
	float sx = 0;
	float sy = 0;

	static const float k = kernel.devScreen.x / kernel.devScreen.y;

	if (kernel.screen.x * (1 / k) <= kernel.screen.y) //Горизонтальные полосы
	{
		if (layer <= 0 || layer > 3)
		{
			sx = posScale.scale.x + (float)0.03 * kernel.factor.x;
			sy = posScale.scale.y + (float)0.03 * kernel.factor.x;
		}
		else
		{
			sx = posScale.scale.x + (float)0.03 * (2 << (layer - 1)) * kernel.factor.x;
			sy = posScale.scale.y + (float)0.03 * (2 << (layer - 1)) * kernel.factor.x;
		}
	}
	else // Вертикальные полосы
	{
		if (layer <= 0 || layer > 3)
		{
			sx = posScale.scale.x + (float)0.03 * kernel.factor.y;
			sy = posScale.scale.y + (float)0.03 * kernel.factor.y;
		}
		else
		{
			sx = posScale.scale.x + (float)0.03 * (2 << (layer - 1)) * kernel.factor.y;
			sy = posScale.scale.y + (float)0.03 * (2 << (layer - 1)) * kernel.factor.y;
		}
	}

	posScale.pos.x = posScale.pos.x - w * (sx - posScale.scale.x) / 2;
	posScale.pos.y = posScale.pos.y - h * (sy - posScale.scale.y) / 2;

	posScale.scale.x = sx;
	posScale.scale.y = sy;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~