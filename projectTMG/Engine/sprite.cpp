//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Sprite::Sprite(std::string src, bool smooth)
{
	if (!setStrTexture(src, smooth))
	std::cout << "ErrorSprite " << src << std::endl;
}
//-----------------------------------------------------------------------------
Sprite::Sprite(SpriteData sd)
{
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
void Sprite::change(SpriteData sd) 
{
	if (!setStrTexture(sd.src, sd.smooth))
	std::cout << "ErrorChangeSprite " << sd.src << std::endl;
	setPosition(sd.x, sd.y);
	setScale(sd.scale, sd.scale);
}
//-----------------------------------------------------------------------------
void Sprite::draw(sf::RenderWindow *win) 
{
	win->draw(*this);
}
//-----------------------------------------------------------------------------