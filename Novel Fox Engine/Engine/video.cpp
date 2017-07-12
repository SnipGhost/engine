//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Video::Video(std::string src, float width, float height, float x, float y, float volume, bool loop)
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
bool Video::setVideo(std::string src, float width, float height, float x, float y, float volume, bool loop)
{
	if (!openFromFile(src)) 
		return 0;
	fit(x, y, width, height);
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
std::ostream &ng::operator << (std::ostream& os, const Video &s)
{
	sf::Vector2f pos = s.getPosition();
	sf::Vector2f scl = s.getScale();
	os << s.id << " [ng::Video]" << std::endl;
	os << "\tLayer: " << s.layer << std::endl;
	os << "\tPosition: (" << pos.x << "; " << pos.y << ")" << std::endl;
	os << "\tScale: (" << scl.x << "; " << scl.y << ")" << std::endl;
	return os;
}
//-----------------------------------------------------------------------------