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

	const float centerX = WS_X / 2;
	const float centerY = WS_Y / 2;
	float mouseXC = centerX - kernel.getMouse().x;
	float mouseYC = centerY - kernel.getMouse().y;

	sf::Vector2f size;
	size.y = (kernel.window->getSize().y - kernel.window->getSize().x * 
		kernel.devScreen.y / kernel.devScreen.x) / 2;
	size.x = (kernel.window->getSize().x - kernel.window->getSize().y * 
		kernel.devScreen.x / kernel.devScreen.y) / 2;

	float k = kernel.devScreen.x / kernel.devScreen.y;

	if ((WS_X * (1 / k) > WS_Y && kernel.getMouse().x > size.x &&
		kernel.getMouse().x < kernel.window->getSize().x - size.x && 
		kernel.getMouse().y > 0 && kernel.getMouse().y < WS_Y) || 
		(WS_X * (1 / k) <= WS_Y  && kernel.getMouse().y > size.y &&
		kernel.getMouse().y < kernel.window->getSize().y - size.y &&
		kernel.getMouse().x > 0 && kernel.getMouse().x < WS_X))
		{

			/*float posX = posScale.pos.x + mouseXC / (40 / (2 << (layer - 1)));		// СТАРАЯ РЕАЛИЗАЦИЯ
			float posY = posScale.pos.y + mouseYC / (40 / (2 << (layer - 1)));*/		// СТАРАЯ РЕАЛИЗАЦИЯ

			/*float sx = posScale.scale.x + (float)0.03 * (2 << (layer - 1));			//ПРИМЕР ИЗ computeLayerScale
			float sy = posScale.scale.y + (float)0.03 * (2 << (layer - 1));*/			//ПРИМЕР ИЗ computeLayerScale

			float posX = posScale.pos.x + mouseXC * (float)0.03 * (2 << (layer - 1));   // ЗАМЕНА БОЛЕЕ ПОХОЖИМ
			float posY = posScale.pos.y + mouseYC * (float)0.03 * (2 << (layer - 1));   // ЗАМЕНА БОЛЕЕ ПОХОЖИМ
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
	static const float k = kernel.devScreen.x / kernel.devScreen.y;

	if (WS_X * (1 / k) <= WS_Y)
	{
		posScale.pos.x = origin.pos.x * KWS_X;
		posScale.pos.y = origin.pos.y * KWS_X + (WS_Y - WS_X * (1 / k)) / 2;

		posScale.scale.x = origin.scale.x * KWS_X;
		posScale.scale.y = origin.scale.y * KWS_X;
	}
	else
	{
		posScale.pos.x = origin.pos.x * KWS_Y + ((WS_X - WS_Y * k) / 2);
		posScale.pos.y = origin.pos.y * KWS_Y;

		posScale.scale.x = origin.scale.x * KWS_Y;
		posScale.scale.y = origin.scale.y * KWS_Y;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~