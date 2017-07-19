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
void Displayable::doLayerMotion(sf::Transformable *obj) // TODO: FIX! [?]
{
	if (layer <= 0) return;

	const float centerX = kernel.screen.x / 2;
	const float centerY = kernel.screen.y / 2;
	float mouseXC = centerX - kernel.getMouse().x;
	float mouseYC = centerY - kernel.getMouse().y;

	sf::Vector2f size;
	size.y = (kernel.window->getSize().y - kernel.window->getSize().x * 
		kernel.devScreen.y / kernel.devScreen.x) / 2;
	size.x = (kernel.window->getSize().x - kernel.window->getSize().y * 
		kernel.devScreen.x / kernel.devScreen.y) / 2;

	float k = (float)16 / 9;

	if ((kernel.screen.x * (1 / k) > kernel.screen.y && kernel.getMouse().x > size.x &&
		kernel.getMouse().x < kernel.window->getSize().x - size.x && 
		kernel.getMouse().y > 0 && kernel.getMouse().y < kernel.screen.y) ||
		(kernel.screen.x * (1 / k) <= kernel.screen.y  && kernel.getMouse().y > size.y &&
		kernel.getMouse().y < kernel.window->getSize().y - size.y &&
		kernel.getMouse().x > 0 && kernel.getMouse().x < kernel.screen.x))
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
// Вычисление новых значений размера
void Displayable::setResize()
{
	static const float k = (float)16 / 9;

	if (kernel.screen.x * (1 / k) <= kernel.screen.y)
	{
		posScale.pos.x = origin.pos.x * kernel.factor.x;
		posScale.pos.y = origin.pos.y * kernel.factor.x + (kernel.screen.y - kernel.screen.x * (1 / k)) / 2;
		//obj->setPosition(posX, pozY);

		posScale.scale.x = origin.scale.x * kernel.factor.x;
		posScale.scale.y = origin.scale.y * kernel.factor.x;
		//obj->setScale(scaleX, scaleY);
	}
	else
	{
		posScale.pos.x = origin.pos.x * kernel.screen.y + ((kernel.screen.x - kernel.screen.y * k) / 2);
		posScale.pos.y = origin.pos.y * kernel.factor.y;
		//obj->setPosition(posX, pozY);

		posScale.scale.x = origin.scale.x * kernel.factor.y;
		posScale.scale.y = origin.scale.y * kernel.factor.y;
		//obj->setScale(scaleX, scaleY);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~