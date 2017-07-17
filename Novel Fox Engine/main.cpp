//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main.cpp										   Главный файл игрового движка
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "Engine/engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main()
{
	XMLNode node = NULL;
	std::vector<Displayable*> layers[C_LAYERS];
	std::map<std::string, ng::Displayable*> objects;
	std::map<std::string, ng::Video*> videos;
	std::map<std::string, ng::Music*> music;
	std::map<std::string, ng::Sound*> sounds;
	typedef std::map<std::string, ng::Video*>::iterator VidIt;
	typedef std::map<std::string, ng::Music*>::iterator MusIt;
	//typedef std::map<std::string, ng::Sound*>::iterator SouIt;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ШРИФТЫ]
	node = parseXML("FONT");
	while (node != NULL)
	{
		FontData data = getFontData(node);
		kernel.fonts[data.id] = new Font(data);
		kernel.print("Loaded font: " + data.src, INFO);
		node = getNextXMLNode(node, "FONT");
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	const size_t COUNT = 6;
	const char *TAGS[COUNT] = { 
		"TEXT", "SPRITE", "ANIMATION", "VIDEO", "MUSIC", "SOUND" 
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (size_t i = 0; i < COUNT; ++i)
	{
		node = parseXML(TAGS[i]);
		while (node != NULL)
		{
			ResData data = getResData(node);
			switch (i) 
			{
				case 0:
				{
					kernel.print(data.layer + MAX_LAYER);
					objects[data.id] = new Text(data);
					layers[data.layer + MAX_LAYER].push_back(objects[data.id]);
					kernel.print(objects[data.id], INFO);
					break;
				}
				case 1:
				{
					kernel.print(data.layer + MAX_LAYER);
					objects[data.id] = new Sprite(data);
					layers[data.layer + MAX_LAYER].push_back(objects[data.id]);
					kernel.print(objects[data.id], INFO);
					break;
				}
				case 2:
				{
					objects[data.id] = new AnimateSprite(data);
					layers[data.layer + MAX_LAYER].push_back(objects[data.id]);
					kernel.print(objects[data.id], INFO);
					break;
				}
				case 3:
				{
					Video *video = new Video(data);
					video->play();
					videos[data.id] = video;
					objects[data.id] = video;
					layers[data.layer + MAX_LAYER].push_back(objects[data.id]);
					kernel.print(video, INFO);
					break;
				}
				case 4:
				{
					Music *mus = new Music(data);
					mus->play();
					music[data.id] = mus;
					kernel.print(mus, INFO);
					break;
				}
				case 5:
				{
					sounds[data.id] = new Sound(data);
					kernel.print(sounds[data.id], INFO);
					break;
				}
			}
			node = getNextXMLNode(node, TAGS[i]);
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	kernel.print("Resources loaded", NORM);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ОГРАНИЧИВАЮЩИЕ ПОЛОСЫ]
	Shape band1(sf::Color::Black, "top-left");
	Shape band2(sf::Color::Black, "bottom-right");
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ГЛАВНЫЙ ЦИКЛ]
	while (kernel.window->isOpen())
	{
		while (kernel.window->pollEvent(kernel.event))
		{
			if (kernel.event.isKeyboardKey(kernel.event.keyboard.Escape) || kernel.event.isWinClosed()) 
				kernel.window->close();
			if (sounds.count("click") > 0 && kernel.event.isMouseClickKey(sf::Mouse::Left)) 
				sounds["click"]->play();
		}

		if (lostFocus())
		{
			if (sounds.count("click") > 0) 
				sounds["click"]->stop();
			for (MusIt it = music.begin(); it != music.end(); ++it)
				it->second->setPause();
			for (VidIt it = videos.begin(); it != videos.end(); ++it)
				it->second->setPause();
			delay(FOCUS_DELAY);
			continue;
		}
		if (hasFocus())
		{
			for (VidIt it = videos.begin(); it != videos.end(); ++it)
				if (!kernel.event.isVideoPlay(*(it->second)))
					it->second->play();
			for (MusIt it = music.begin(); it != music.end(); ++it)
				if (!kernel.event.isMusicPlay(*it->second)) 
					it->second->play();
		}

		startDisplay();

		//for (auto it = objects.begin(); it != objects.end(); ++it)
		//{
		//	it->second->setLayerMotion();
		//	it->second->display();
		//}

		for (auto &layer : layers)
		{
			for (auto &obj : layer)
			{
				obj->setLayerMotion();
				obj->display();
			}
		}

		band1.display();
		band2.display();

		endDisplay();
		//delay(1);
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (auto &layer : layers)
	{
		for (auto &obj : layer)
		{
			kernel.print("Deleting objects: " + obj->getId(), INFO);
			delete obj;
			obj = NULL;
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	return EXIT_SUCCESS;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
