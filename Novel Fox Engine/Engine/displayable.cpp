//-----------------------------------------------------------------------------
// displayable.cpp                            ���������� ������ displayable.cpp
//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
void Displayable::doLayerMotion(sf::Transformable *obj)
{
	const float centerX = WS_X / 2;
	const float centerY = WS_Y / 2;
	float mouseXC = centerX - kernel.mouse().x; //���������� ���� �� X
	float mouseYC = centerY - kernel.mouse().y; //���������� ���� �� Y

	if (layer >0 && kernel.mouse().x > 0 && kernel.mouse().x < WS_X && 
        kernel.mouse().y > 0 && kernel.mouse().y < WS_Y)
    {
		float posX = pos.x + mouseXC / (40 / (2 << (layer-1)));
		float posY = pos.y + mouseYC / (40 / (2 << (layer-1)));
		obj->setPosition(posX, posY);
    }
}
//-----------------------------------------------------------------------------
unsigned int Displayable::getLayer() 
{ 
	return layer; 
}
//-----------------------------------------------------------------------------