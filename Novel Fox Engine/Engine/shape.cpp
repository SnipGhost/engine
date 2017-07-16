//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// shape.cpp                                           Реализация класса шейпов
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Shape::Shape(sf::Color color, std::string pos)
{
	float k = (float)16 / 9;

	if (WS_X * (1 / k) <= WS_Y)
	{
		size.y = (kernel.window->getSize().y - kernel.window->getSize().x *
			kernel.devScreen.y / kernel.devScreen.x) / 2;
		size.x = (float)kernel.window->getSize().x;

		if (pos == "top")
			setSize(sf::Vector2f(size.x, size.y));
		else if (pos == "bottom")
		{
			setSize(sf::Vector2f(size.x, size.y));
			setPosition(0, kernel.window->getSize().y - size.y);
		}
	}
	else
	{
		size.x = (kernel.window->getSize().x - kernel.window->getSize().y *
			kernel.devScreen.x / kernel.devScreen.y) / 2;
		size.y = (float)kernel.window->getSize().y;

		if (pos == "top")
			setSize(sf::Vector2f(size.x, size.y));
		else if (pos == "bottom")
		{
			setSize(sf::Vector2f(size.x, size.y));
			setPosition(kernel.window->getSize().x - size.x, 0);
		}
	}
	setFillColor(color);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Shape::display(sf::RenderWindow *win)
{
	win->draw(*this);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~