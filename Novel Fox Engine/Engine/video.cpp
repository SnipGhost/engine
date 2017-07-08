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
Video::Video(VideoData vd)
{
	if (!setVideo(vd.src, vd.width, vd.height, vd.x, vd.y, vd.volume, vd.loop))
		kernel.print("Failed load video " + vd.src, WARN);
	else
		kernel.print("Created video " + vd.src, INFO);
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
	{
		play();
	}
}
//-----------------------------------------------------------------------------
void Video::display(sf::RenderWindow *win)
{
	setLoop(loopVideo);
	if (getStatus() != sfe::Paused)
	{
		update();
		win->draw(*this);
	}
	if (getStatus() == sfe::Playing) //Åñëè âèäåî stop, òî no Draw //ÎØÈÁÊÀ Â ÊÎÍÑÎËÅ!
	{
		win->draw(*this);
	}
}
//-----------------------------------------------------------------------------