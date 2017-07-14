//-----------------------------------------------------------------------------
// main.cpp														 DEMO-Программа
//-----------------------------------------------------------------------------
#include "Engine/engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
int main()
{
	XMLNode node = NULL;
	std::map<std::string, ng::Video*> videos;
	std::map<std::string, ng::Displayable*> objects;
	typedef std::map<std::string, ng::Displayable*>::iterator ObjIt;
	typedef std::map<std::string, ng::Video*>::iterator VidIt;
	Music *music = NULL;
	Sound *sound = NULL;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ШРИФТЫ]
	node = parseXML("FONT");
	while (node != NULL)
	{
		FontData data = getFontData(node);
		Font *font = new Font(data);
		kernel.fonts[data.id] = font;
		kernel.print("Loaded font: " + data.src, INFO);
		node = getNextXMLNode(node, "FONT");
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	const size_t COUNT = 6;
	const char *TAGS[COUNT] = {"TEXT", "SPRITE", "ANIMATION", "VIDEO", "MUSIC", "SOUND"};
	for (int i = 0; i < COUNT; ++i)
	{
		node = parseXML(TAGS[i]);
		while (node != NULL)
		{
			ResData data = getResData(node);
			switch (i) 
			{
				case 0:
				{
                    Text *text = new Text(data);
                    objects[data.id] = text;
                    setResize((sf::Transformable*)text);
					kernel.print(text, INFO);
					break;
				}
				case 1:
				{
                    Sprite *sprite = new Sprite(data);
					objects[data.id] = sprite;
                    setResize((sf::Transformable*)sprite);
					kernel.print(sprite, INFO);
					break;
				}
				case 2:
				{
                    AnimateSprite *a =new AnimateSprite(data);
					objects[data.id] = a;
                    setResize((sf::Transformable*)a);
					kernel.print(a, INFO);
					break;
				}
				case 3:
				{
					Video *video = new Video(data);
					video->play();
					videos[data.id] = video;
					objects[data.id] = video;
                    setResize(video);
					kernel.print(video, INFO);
					break;
				}
				case 4:
				{
					music = new Music(data);
					music->play();
					kernel.print(music, INFO);
					break;
				}
				case 5:
				{
					sound = new Sound(data);
					kernel.print(sound, INFO);
					break;
				}
			}
			node = getNextXMLNode(node, TAGS[i]);
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	kernel.print("Resources loaded.", NORM);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	while (kernel.window->isOpen())
	{
		while (kernel.window->pollEvent(kernel.event))
		{
			if (kernel.event.isKeyboardKey(kernel.event.keyboard.Escape) ||
				kernel.event.isWinClosed()) kernel.window->close();
			if (kernel.event.isMouseClickKey(sf::Mouse::Left)) sound->play();
		}
		if (kernel.event.isMouseKey(sf::Mouse::Right)) music->setStop();

		if (kernel.event.type == sf::Event::LostFocus || !kernel.window->hasFocus())
		{ 
			music->setPause();
			sound->stop();
			for (VidIt it = videos.begin(); it != videos.end(); ++it)
				it->second->setPause();
			delay(FOCUS_DELAY);
			continue;
		}
		if (kernel.window->hasFocus())
		{
			for (VidIt it = videos.begin(); it != videos.end(); ++it)
				if (!kernel.event.isVideoPlay(*(it->second)))
					it->second->play();
			if (!kernel.event.isMusicPlay(*music)) music->play();
		}

		startDisplay();

		for (ObjIt it = objects.begin(); it != objects.end(); ++it)
			it->second->display();

		endDisplay();
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (ObjIt it = objects.begin(); it != objects.end(); ++it)
	{
		kernel.print("Deleting objects: " + it->first, INFO);
		delete it->second;
		it->second = NULL;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------