//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Music::Music(std::string src, float volume, bool loop)
{
	if (!setMusic(src, volume, loop))
		kernel.print("Failed load music " + src, WARN);
	else
		kernel.print("Created music " + src, INFO);
}
//-----------------------------------------------------------------------------
Music::Music(MusicData md)
{
	if (!setMusic(md.src, md.volume, md.loop))
		kernel.print("Failed load music " + md.src, WARN);
	else
		kernel.print("Created music " + md.src, INFO);
}
//-----------------------------------------------------------------------------
bool Music::setMusic(std::string src, float volume, bool loop)
{
	if (!openFromFile(src)) 
		return 0;
	setVolume(volume);
	setLoop(loop);
	return 1;
}
//-----------------------------------------------------------------------------
void Music::setStop()
{
	if (getStatus() == sf::Music::Playing) {
		volume = getVolume();
		volume-=1;
		if (volume <= 0)	
			stop();
		else 
			setVolume(volume);
	}
}
//-----------------------------------------------------------------------------
void Music::setPause()
{
	if (getStatus() == sf::Music::Playing)
		pause();
}
//-----------------------------------------------------------------------------
//void Music::change() 
//{
//
//}
//-----------------------------------------------------------------------------
std::ostream &ng::operator << (std::ostream &os, const Music &m)
{
	os << "[ng::Music]" << std::endl;
	os << "\tDuration (sec):\t" << m.getDuration().asSeconds() << std::endl;
	os << "\tChannel count: \t" << m.getChannelCount() << std::endl;
	os << "\tVolume level:  \t" << m.getVolume() << std::endl;
	os << "\tLooping value: \t" << m.getLoop() << std::endl;
	return os;
}
//-----------------------------------------------------------------------------