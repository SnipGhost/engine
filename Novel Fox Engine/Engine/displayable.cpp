//-----------------------------------------------------------------------------
// displayable.cpp                            Реализация класса displayable.cpp
//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
void Displayable::doLayerMotion(sf::Transformable *obj)
{
	const float centerX = WS_X / 2;
	const float centerY = WS_Y / 2;
	float mouseXC = centerX - kernel.mouse().x; //Отклонение мыши по X
	float mouseYC = centerY - kernel.mouse().y; //Отклонение мыши по Y

	if (kernel.mouse().x > 0 && kernel.mouse().x < WS_X && kernel.mouse().y > 0 && kernel.mouse().y < WS_Y)
		if (layer > 0)
		{
			float posX = pos.x + mouseXC / (40 / std::pow((float)2, layer - 1));
			float posY = pos.y + mouseYC / (40 / std::pow((float)2, layer - 1));
			obj->setPosition(posX, posY);
		}
}
//-----------------------------------------------------------------------------
unsigned int Displayable::getLayer() 
{ 
	return layer; 
}
//-----------------------------------------------------------------------------