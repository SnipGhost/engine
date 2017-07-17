//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// displayable.cpp                    Реализация класса отрисовываемых объектов
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Displayable::doLayerMotion(sf::Transformable *obj)
{
	const float centerX = WS_X / 2;
	const float centerY = WS_Y / 2;
	float mouseXC = centerX - kernel.mouse().x;
	float mouseYC = centerY - kernel.mouse().y;

	sf::Vector2f size;
	size.y = (kernel.window->getSize().y - kernel.window->getSize().x * 
		kernel.devScreen.y / kernel.devScreen.x) / 2;
	size.x = (kernel.window->getSize().x - kernel.window->getSize().y * 
		kernel.devScreen.x / kernel.devScreen.y) / 2;


	float k = (float)16 / 9;

	if (layer > 0 && ((WS_X * (1 / k) > WS_Y && kernel.mouse().x > size.x &&
		kernel.mouse().x < kernel.window->getSize().x - size.x && 
		kernel.mouse().y > 0 && kernel.mouse().y < WS_Y) || 
		(WS_X * (1 / k) <= WS_Y  && kernel.mouse().y > size.y &&
		kernel.mouse().y < kernel.window->getSize().y - size.y &&
		kernel.mouse().x > 0 && kernel.mouse().x < WS_X)))
		{
			float posX = posScale.pos.x + mouseXC / (40 / (2 << (layer - 1)));
			float posY = posScale.pos.y + mouseYC / (40 / (2 << (layer - 1)));
			obj->setPosition(posX, posY);
		}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int Displayable::getLayer() 
{ 
	return layer; 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~