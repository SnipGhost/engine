//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// shape.cpp                                           Реализация класса шейпов
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Shape::Shape(sf::Color color, std::string pos, 
	         sf::Vector2f size, sf::Vector2f devSize)
{
	const float k = (float) 16 / 9;

	if (size.x * (1 / k) <= size.y)
	{
		size.y = (size.y - size.x * devSize.y / devSize.x) / 2;
		size.x = (float)size.x;
		if (pos == "bottom-right") setPosition(0, size.y - size.y);
	}
	else
	{
		size.x = (size.x - size.y * devSize.x / devSize.y) / 2;
		size.y = (float)size.y;
		if (pos == "bottom-right") setPosition(size.x - size.x, 0);
	}
	setSize(size);
	setFillColor(color);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Shape::display(sf::RenderWindow *win)
{
	win->draw(*this);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~