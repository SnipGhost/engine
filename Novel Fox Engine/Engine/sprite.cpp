//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sprite.cpp                                         Реализация класса спрайта
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sprite::Sprite(std::string id, std::string src, bool smooth) //Нигде не юзаем [!]
{
	if (!setStrTexture(src, smooth))
		kernel.print("Failed to create sprite " + id, WARN);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sprite::Sprite(ResData rd)
{
	if (!setStrTexture(rd.src, rd.smooth))
		kernel.print("Failed to create sprite " + rd.id, WARN);

	if (GETBIT(rd.bitMask, _alpha)) setAlpha(rd.alpha);
	if (GETBIT(rd.bitMask, _angle)) setRotation(rd.angle);

	id = rd.id;
	layer = rd.layer;
	layermotion = rd.layermotion;
	visible = rd.visible;
	positionObj = sf::Vector2f(rd.x, rd.y);
	scaleObj = rd.scale;
	angle = rd.angle;
	style = rd.style;

	setRenderStates(style);

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
void Sprite::setAlpha(int alpha)
{
	color.a = alpha;
	setColor(sf::Color(255, 255, 255, color.a));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Sprite::setRenderStates(std::string style)
{
	if (style == "blendadd")
	{
		renderStates.blendMode = sf::BlendAdd;	  //Photoshop: Линейный осветлитель (добавить)
	}
	else if (style == "blendmultiply")
	{
		renderStates.blendMode = sf::BlendMultiply; //Photoshop: Умножение
	}
	else 
		renderStates = sf::RenderStates::Default;

	/*renderStates.shader = &shader;*/
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Sprite::isMouseAbove() // Эта функция работает верно
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
void Sprite::edit(ResData rd)
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
	kernel.print("Edit mode for sprite: " + rd.id, INFO);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Sprite::display(sf::RenderWindow *win) 
{
	win->draw(*this, renderStates);
	if (this->isMouseAbove() && (id == "yes" || id == "no")) // ТЕСТ [!]
	{
		ng::Shape sp(sf::Vector2f(getTextureRect().width * posScale.scale.x, getTextureRect().height * posScale.scale.y), sf::Vector2f(posScale.pos.x, posScale.pos.y));
		win->draw(sp);
	}
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
	int w = getTextureRect().width;
	int h = getTextureRect().height;
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