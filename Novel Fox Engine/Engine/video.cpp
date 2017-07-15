//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Video::Video(std::string src, int width, int height, float x, float y, float volume, bool loop)
{
	if (!setVideo(src, width, height, x, y, volume, loop))
		kernel.print("Failed load video " + src, WARN);
	else
		kernel.print("Created video " + src, INFO);
}
//-----------------------------------------------------------------------------
Video::Video(ResData rd)
{
	if (!setVideo(rd.src, rd.width, rd.height, rd.x, rd.y, rd.volume, rd.loop))
		kernel.print("Failed load video " + rd.id, WARN);
	else
		kernel.print("Created video " + rd.id, INFO);
	id = rd.id;
	layer = rd.layer;
}
//-----------------------------------------------------------------------------
bool Video::setVideo(std::string src, int width, int height, float x, float y, float volume, bool loop)
{
	if (!openFromFile(src)) 
		return 0;
	fit(x, y, (float)width, (float)height);
	setVolume(volume);
	loopVideo = loop;
	return 1;
}
//-----------------------------------------------------------------------------
void Video::setLoop(bool loop)
{
	if (loop == true && getStatus() == sfe::Stopped)
		play();
}
//-----------------------------------------------------------------------------
void Video::setPause()
{
	if (getStatus() == sfe::Playing)
		pause();
}
//-----------------------------------------------------------------------------
void Video::display(sf::RenderWindow *win)
{
	setLoop(loopVideo);
	if (getStatus() == sfe::Playing)
	{
		update();
		win->draw(*this);
	}
}
//-----------------------------------------------------------------------------
std::ostream &ng::operator << (std::ostream& os, Video &v)
{
	return v.print(os);
}
//-----------------------------------------------------------------------------
std::ostream & Video::print(std::ostream &os)
{
	sf::Vector2f pos = getPosition();
	sf::Vector2f scl = getScale();
	os << id << " [ng::Video]" << std::endl;
	os << "\tLayer:   \t" << layer << std::endl;
	os << "\tPosition:\t(" << pos.x << "; " << pos.y << ")" << std::endl;
	os << "\tScale:   \t(" << scl.x << "; " << scl.y << ")" << std::endl;
	os << "\tReSize:  \t(" << KWS_X << "; " << KWS_Y << ")" << std::endl;
	return os;
}
//-----------------------------------------------------------------------------
void Video::setResize()
{
	pos = ng::setResize(this);
}
//-----------------------------------------------------------------------------
void Video::setLayerMotion()
{
	doLayerMotion(this);
}
//-----------------------------------------------------------------------------