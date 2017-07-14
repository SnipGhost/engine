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
	// setResize(); //[!]
}
//-----------------------------------------------------------------------------
Video::Video(ResData rd)
{
	if (!setVideo(rd.src, rd.width, rd.height, rd.x, rd.y, rd.volume, rd.loop))
		kernel.print("Failed load video «" + rd.id + "»", WARN);
	else
		kernel.print("Created video «" + rd.id + "»", INFO);
	id = rd.id;
	layer = rd.layer;
	// setResize(); //[!]
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
// void Video::setResize()
// {
// 	float x = 1280;
// 	float y = 720;
// 	float k = x / y;

// 	if (WS_X*(1 / k) <= WS_Y)
// 	{
// 		setPosition(getPosition().x*KWS_X, getPosition().y*KWS_X + (WS_Y - WS_X * (1.0 / k)) / 2);
// 		setScale(getScale().x*KWS_X, getScale().y*KWS_X);
// 	}
// 	else
// 	{
// 		setPosition(getPosition().x*KWS_Y + ((WS_X - WS_Y * k) / 2), getPosition().y*KWS_Y);
// 		setScale(getScale().x*KWS_Y, getScale().y*KWS_Y);
// 	}
// }
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
std::ostream &ng::operator << (std::ostream& os, const Video &s)
{
	sf::Vector2f pos = s.getPosition();
	sf::Vector2f scl = s.getScale();
	os << s.id << " [ng::Video]" << std::endl;
	os << "\tLayer:  \t" << s.layer << std::endl;
	os << "\tPosition: \t(" << pos.x << "; " << pos.y << ")" << std::endl;
	os << "\tScale:  \t(" << scl.x << "; " << scl.y << ")" << std::endl;
	return os;
}
//-----------------------------------------------------------------------------