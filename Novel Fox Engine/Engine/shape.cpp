//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// shape.cpp                                           Реализация класса шейпов
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Shape::Shape(sf::Color color, std::string pos, 
	         sf::Vector2f winSize, sf::Vector2f devSize)
{
	const float k = (float)16 / 9;

	if (winSize.x * (1 / k) <= winSize.y)
	{
		size.y = (winSize.y - winSize.x * devSize.y / devSize.x) / 2;
		size.x = (float)winSize.x;
		if (pos == "bottom-right") setPosition(0, winSize.y - size.y);
	}
	else
	{
		size.x = (winSize.x - winSize.y * devSize.x / devSize.y) / 2;
		size.y = (float)winSize.y;
		if (pos == "bottom-right") setPosition(winSize.x - size.x, 0);
	}
	setSize(sf::Vector2f(size.x, size.y));
	setFillColor(color);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Shape::display(sf::RenderWindow *win)
{
	win->draw(*this);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~