//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// video.cpp                                            Реализация класса видео
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Video::Video(std::string src, int width, int height, float x, float y, 
	float volume, bool loop)
{
	if (!setVideo(src, width, height, x, y, volume, loop))
		kernel.print("Failed load video " + src, WARN);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Video::Video(ResData rd)
{
	if (!setVideo(rd.src, rd.width, rd.height, rd.x, rd.y, rd.volume, rd.loop))
		kernel.print("Failed load video " + rd.id, WARN);

	id = rd.id;
	layer = rd.layer;
	layermotion = rd.layermotion;
	visible = rd.visible;
	positionObj = sf::Vector2f(rd.x, rd.y);
	scaleObj = rd.scale;
	style = rd.style;
	height = rd.height;
	width = rd.width;

	setBlendMode(style);

	origin = PosScale(rd.x, rd.y, rd.scale, rd.scale);
	setResize();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Video::setVideo(std::string src, int width, int height, float x, float y, 
	                 float volume, bool loop)
{
	if (!openFromFile(src))
		return 0;
	fit(x, y, (float)width, (float)height);
	setVolume(volume);
	loopVideo = loop;
	return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Video::setLoop(bool loop)
{
	if (loop == true && getStatus() == sfe::Stopped)
		play();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Video::setPause()
{
	if (getStatus() == sfe::Playing)
		pause();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Video::isMouseAbove() // Не проверено на работоспособность [!]
{ 
	if (kernel.getMouse().x >= posScale.pos.x &&
		kernel.getMouse().x < (posScale.pos.x + width * posScale.scale.x) &&
		kernel.getMouse().y >= posScale.pos.y &&
		kernel.getMouse().y < (posScale.pos.y + height * posScale.scale.y))
	{
		return 1;
	}
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Video::edit(ResData rd)
{
	//Сделать Edit для видео
	//~~~~ Не проверено на работоспособность [!]
	if (GETBIT(rd.bitMask, _visible))
	{
		visible = rd.visible;
	}
	if (GETBIT(rd.bitMask, _x) || GETBIT(rd.bitMask, _y))
	{
		if (GETBIT(rd.bitMask, _x))
		{
			positionObj.x = rd.x;
			setPosition(rd.x, positionObj.y);
			origin = PosScale(rd.x, positionObj.y, scaleObj, scaleObj);
		}
		if (GETBIT(rd.bitMask, _y))
		{
			positionObj.y = rd.y;
			setPosition(positionObj.x, rd.y);
			origin = PosScale(positionObj.x, rd.y, scaleObj, scaleObj);
		}
		setResize();
	}
	if (GETBIT(rd.bitMask, _width) || GETBIT(rd.bitMask, _height))
	{
		if (GETBIT(rd.bitMask, _width))
		{
			width = rd.width;
		}
		if (GETBIT(rd.bitMask, _height))
		{
			height = rd.height;
		}
		fit(positionObj.x, positionObj.y, (float)width, (float)height);
	}
	if (GETBIT(rd.bitMask, _scale))
	{
		scaleObj = rd.scale;
		setScale(rd.scale, rd.scale);
		origin = PosScale(positionObj.x, positionObj.y, rd.scale, rd.scale);
		setResize();
	}
	if (GETBIT(rd.bitMask, _volume))
	{
		setVolume(rd.volume);
	}
	//~~~~ Не проверено на работоспособность [!]
	if (GETBIT(rd.bitMask, _style))
	{
		style = rd.style;
		setBlendMode(rd.style);
	}
	kernel.print("Edit mode for video: " + rd.id, INFO);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Video::setBlendMode(std::string style)
{
	if (style == "blendadd")
		renderStates = sf::BlendAdd;	  //Photoshop: Линейный осветлитель (добавить)
	else if (style == "blendmultiply")
		renderStates = sf::BlendMultiply; //Photoshop: Умножение
	else
		renderStates = sf::RenderStates::Default;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Video::display(sf::RenderWindow *win)
{
	setLoop(loopVideo); // Лаги возникают вероятнее всего от множества проверок состояния
	if (getStatus() == sfe::Playing) // Лаги возникают вероятнее всего от множества проверок состояния
	{
		update();
		win->draw(*this, renderStates);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::ostream &ng::operator << (std::ostream& os, Video &v)
{
	return v.print(os);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::ostream & Video::print(std::ostream &os)
{
	sf::Vector2f pos = getPosition();
	sf::Vector2f scl = getScale();
	os << id << " [ng::Video]" << std::endl;
	os << "\tLayer:   \t" << layer << std::endl;
	os << "\tPosition:\t(" << pos.x << "; " << pos.y << ")" << std::endl;
	os << "\tScale:   \t(" << scl.x << "; " << scl.y << ")" << std::endl;
	return os;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Video::setResize()
{
	Displayable::setResize();
	computeLayerScale();
	setPosition(posScale.pos);
	setScale(posScale.scale);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Video::setLayerMotion()
{
	doLayerMotion(this);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Video::computeLayerScale()
{
	float w = this->getSize().x;
	float h = this->getSize().y;
	float sx = 0;
	float sy = 0;

	static const float k = kernel.devScreen.x / kernel.devScreen.y;

	if (kernel.screen.x * (1 / k) <= kernel.screen.y) //Горизонтальные полосы
	{
		if (layer <= 0 || layer > 3)
		{
			sx = posScale.scale.x + (float)0.03 * kernel.factor.x;
			sy = posScale.scale.y + (float)0.03 * kernel.factor.x;
		}
		else
		{
			sx = posScale.scale.x + (float)0.03 * (2 << (layer - 1)) * kernel.factor.x;
			sy = posScale.scale.y + (float)0.03 * (2 << (layer - 1)) * kernel.factor.x;
		}
	}
	else // Вертикальные полосы
	{
		if (layer <= 0 || layer > 3)
		{
			sx = posScale.scale.x + (float)0.03 * kernel.factor.y;
			sy = posScale.scale.y + (float)0.03 * kernel.factor.y;
		}
		else
		{
			sx = posScale.scale.x + (float)0.03 * (2 << (layer - 1)) * kernel.factor.y;
			sy = posScale.scale.y + (float)0.03 * (2 << (layer - 1)) * kernel.factor.y;
		}
	}

	posScale.pos.x = posScale.pos.x - w * (sx - posScale.scale.x) / 2;
	posScale.pos.y = posScale.pos.y - h * (sy - posScale.scale.y) / 2;

	posScale.scale.x = sx;
	posScale.scale.y = sy;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~