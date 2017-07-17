//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// displayable.cpp                    Реализация класса отрисовываемых объектов
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Displayable::~Displayable()
{
	kernel.print("Deleted displayble object: " + id, INFO);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Displayable::doLayerMotion(sf::Transformable *obj)
{
	if (layer <= 0) return;

	const float centerX = WS_X / 2;
	const float centerY = WS_Y / 2;
	float mouseXC = centerX - kernel.getMouse().x;
	float mouseYC = centerY - kernel.getMouse().y;

	sf::Vector2f size;
	size.y = (kernel.window->getSize().y - kernel.window->getSize().x * 
		kernel.devScreen.y / kernel.devScreen.x) / 2;
	size.x = (kernel.window->getSize().x - kernel.window->getSize().y * 
		kernel.devScreen.x / kernel.devScreen.y) / 2;


	float k = (float)16 / 9;

	if ((WS_X * (1 / k) > WS_Y && kernel.getMouse().x > size.x &&
		kernel.getMouse().x < kernel.window->getSize().x - size.x && 
		kernel.getMouse().y > 0 && kernel.getMouse().y < WS_Y) || 
		(WS_X * (1 / k) <= WS_Y  && kernel.getMouse().y > size.y &&
		kernel.getMouse().y < kernel.window->getSize().y - size.y &&
		kernel.getMouse().x > 0 && kernel.getMouse().x < WS_X))
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
std::string Displayable::getId()
{
	return id;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~