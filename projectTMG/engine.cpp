#include "engine.hpp"

using namespace ng;

SpriteData ng::getSpriteData(tinyxml2::XMLElement *spriteNode)
{
	const char *id = spriteNode->Attribute("id");
	const char *src = spriteNode->Attribute("src");
	const char *smooth = spriteNode->Attribute("smooth");
	const char *layer = spriteNode->Attribute("layer");
	SpriteData res;
	res.id = std::string(id);
	res.src = std::string(src);
	res.layer = std::atoi(layer);
	res.smooth = ((strcmp(smooth, "true") == 0) ? true : false);
	return res;
}

Sprite::Sprite(std::string id, std::string src, bool smooth)
{
	this->id = id;
	if (!setStrTexture(src, smooth))
		std::cout << "Error" << std::endl;
}

Sprite::Sprite(ng::SpriteData sd)
{
	this->id = sd.id;
	if (!setStrTexture(sd.src, sd.smooth))
		std::cout << "Error" << std::endl;
}

bool Sprite::setStrTexture(std::string src, bool smooth)
{
	if (!texture.loadFromFile(src))
	{
		std::cout << "Error with texture" << std::endl;
		return 0;
	}
	std::cout << src << std::endl;
	texture.setSmooth(smooth);
	setTexture(texture);
	return 1;
}

//void Sprite::change(tinyxml2::XMLElement) 
//{
//
//}