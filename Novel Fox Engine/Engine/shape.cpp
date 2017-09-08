//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// shape.cpp                                           Реализация класса шейпов
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Shape::Shape(sf::Vector2f winSize)
{
	alpha = 0;
	setSize(sf::Vector2f(winSize.x, winSize.y));
	setFillColor(sf::Color(0, 0, 0, alpha));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Shape::Shape(sf::Color clr, int pos, sf::Vector2f winSize, sf::Vector2f devSize)
{
	const float k = (float)16 / 9;

	if (winSize.x * (1 / k) <= winSize.y)
	{
		size.y = (winSize.y - winSize.x * devSize.y / devSize.x) / 2;
		size.x = (float)winSize.x;
		if (pos == 2) setPosition(0, winSize.y - size.y);
	}
	else
	{
		size.x = (winSize.x - winSize.y * devSize.x / devSize.y) / 2;
		size.y = (float)winSize.y;
		if (pos == 2) setPosition(winSize.x - size.x, 0);
	}
	setSize(sf::Vector2f(size.x, size.y));
	setFillColor(clr);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Shape::Shape(sf::Vector2f size, sf::Vector2f pos)
{
	setPosition(pos.x, pos.y);
	setSize(sf::Vector2f(size.x, size.y));
	setFillColor(sf::Color(0, 0, 255, 128));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Shape::getAlpha()
{
	return alpha;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Shape::addAlpha()
{
	alpha += 1;
	setFillColor(sf::Color(0, 0, 0, alpha));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Shape::pickUpAlpha()
{
	alpha -= 1;
	setFillColor(sf::Color(0, 0, 0, alpha));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Shape::display(sf::RenderWindow *win)
{
	win->draw(*this);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~