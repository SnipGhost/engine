//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Video::Video(std::string src, float width, float height, float x, float y, float volume)
{
	if (!setVideo(src, width, height, x, y, volume))
		kernel.print("Failed load video " + src, WARN);
	else
		kernel.print("Created video " + src, INFO);
}
//-----------------------------------------------------------------------------
Video::Video(VideoData vd)
{
	if (!setVideo(vd.src, vd.width, vd.height, vd.x, vd.y, vd.volume))
		kernel.print("Failed load video " + vd.src, WARN);
	else
		kernel.print("Created video " + vd.src, INFO);
}
//-----------------------------------------------------------------------------
bool Video::setVideo(std::string src, float width, float height, float x, float y, float volume)
{
	if (!openFromFile(src)) 
		return 0;
	fit(x, y, width, height);
	setVolume(volume);
	return 1;
}
//-----------------------------------------------------------------------------
void Video::display(sf::RenderWindow *win)
{
	if (getStatus() == sfe::Playing) //Åñëè âèäåî stop, òî no Draw //ÎØÈÁÊÀ Â ÊÎÍÑÎËÅ!
	{
		update();
		win->draw(*this);
	}
}
//-----------------------------------------------------------------------------