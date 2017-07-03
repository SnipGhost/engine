//-----------------------------------------------------------------------------
// engine.cpp												  Реализации Engine
//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
SpriteData ng::getSpriteData(tinyxml2::XMLElement *spNode, std::string path)
{
	const char *id = spNode->Attribute("id");
	const char *x = spNode->Attribute("x");
	const char *y = spNode->Attribute("y");
	const char *scale = spNode->Attribute("scale");
	const char *src = spNode->Attribute("src");
	const char *smooth = spNode->Attribute("smooth");
	const char *layer = spNode->Attribute("layer");
	SpriteData res;
	res.id = std::string(id);
	res.src = path + std::string(src);
	res.x = std::stof(x);
	res.y = std::stof(y);
	res.scale = std::stof(scale);
	res.layer = std::atoi(layer);
	res.smooth = ((strcmp(smooth, "true") == 0) ? true : false);
	return res;
}
//-----------------------------------------------------------------------------
Sprite::Sprite(std::string id, std::string src, bool smooth)
{
	this->id = id;
	if (!setStrTexture(src, smooth))
		std::cout << "Error" << std::endl;
}
//-----------------------------------------------------------------------------
Sprite::Sprite(ng::SpriteData sd)
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