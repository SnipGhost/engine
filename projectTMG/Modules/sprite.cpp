#include "engine.hpp"
using namespace ng;
//-----------------------------------------------------------------------------
Sprite::Sprite(std::string id, std::string src, bool smooth)
{
	this->id = id;
	if (!setStrTexture(src, smooth))
	std::cout << "Error" << std::endl;
}
//-----------------------------------------------------------------------------
Sprite::Sprite(SpriteData sd)
{
	this->id = sd.id;
	if (!setStrTexture(sd.src, sd.smooth))
	std::cout << "Error" << std::endl;
	setPosition(sd.x, sd.y);
	setScale(sd.scale, sd.scale);
}
//-----------------------------------------------------------------------------
bool Sprite::setStrTexture(std::string src, bool smooth)
{
	if (!texture.loadFromFile(src))
	return 0;
	texture.setSmooth(smooth);
	setTexture(texture);
	return 1;
}
//-----------------------------------------------------------------------------
//void Sprite::change(tinyxml2::XMLElement) 
//{
//
//}
//-----------------------------------------------------------------------------