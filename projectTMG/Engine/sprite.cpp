//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Sprite::Sprite(std::string id, std::string src, bool smooth)
{
	this->id = id; //TO DO
	if (!setStrTexture(src, smooth))
	std::cout << "ErrorSprite " << src << std::endl;

	saveTime = 0;
	saveFrame = 1;

	sideSize = getTextureRect().height;
}
//-----------------------------------------------------------------------------
Sprite::Sprite(SpriteData sd)
{
	this->id = sd.id; //TO DO
	if (!setStrTexture(sd.src, sd.smooth))
	std::cout << "ErrorSprite " << sd.src << std::endl;
	setPosition(sd.x, sd.y);
	setScale(sd.scale, sd.scale);
}
//-----------------------------------------------------------------------------
bool Sprite::setStrTexture(std::string src, bool smooth)
{
	if (!texture.loadFromFile(src)) return 0;
	texture.setSmooth(smooth);
	setTexture(texture);
	return 1;
}
//-----------------------------------------------------------------------------
void Sprite::setAnimation(int time) 
{
	    if ((time-saveTime) >= 40) //ÌÈËËÈÑÅÊÓÍÄÛ ~25 FPS
		{
			saveTime = time;
			saveFrame++;
			if (saveFrame * sideSize >= this->getTexture()->getSize().x)
				saveFrame = 0;
		}
		setTextureRect(sf::IntRect(sideSize * saveFrame, 0, sideSize, sideSize));
}
//-----------------------------------------------------------------------------
void Sprite::change(SpriteData sd) 
{
	if (!setStrTexture(sd.src, sd.smooth))
	std::cout << "ErrorChangeSprite " << sd.src << std::endl;
	setPosition(sd.x, sd.y);
	setScale(sd.scale, sd.scale);
}
//-----------------------------------------------------------------------------