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
void Video::edit(ResData rd)
{
	//Сделать Edit для видео
	kernel.print("Edit mode for video: " + rd.id, INFO);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Video::display(sf::RenderWindow *win)
{
	setLoop(loopVideo);
	if (getStatus() == sfe::Playing)
	{
		update();
		win->draw(*this);
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