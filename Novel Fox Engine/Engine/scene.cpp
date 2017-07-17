//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// engine.cpp                         Реализации ядра и вспомогательных функций
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ng::Scene::loadScene()
{
	const size_t COUNT = 6;
	const char *TAGS[COUNT] = {
		"TEXT", "SPRITE", "ANIMATION", "VIDEO", "MUSIC", "SOUND"
	};
	XMLNode node = NULL;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (size_t i = 0; i < COUNT; ++i)
	{
		node = kernel.parseXML(TAGS[i]);
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
			node = kernel.getNextXMLNode(node, TAGS[i]);
		}
	}
	kernel.print("Resources loaded", NORM);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ng::Scene::startMedia()
{
	for (VidIt it = videos.begin(); it != videos.end(); ++it)
		if (!kernel.event.isVideoPlay(*(it->second)))
			it->second->play();
	for (MusIt it = music.begin(); it != music.end(); ++it)
		if (!kernel.event.isMusicPlay(*it->second))
			it->second->play();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ng::Scene::stopMedia()
{
	if (sounds.count("click") > 0)
		sounds["click"]->stop();
	for (MusIt it = music.begin(); it != music.end(); ++it)
		it->second->setPause();
	for (VidIt it = videos.begin(); it != videos.end(); ++it)
		it->second->setPause();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ng::Scene::displayAll()
{
	for (auto &layer : layers)
	{
		for (auto &obj : layer)
		{
			obj->setLayerMotion();
			obj->display();
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ng::Scene::clear()
{
	for (auto &layer : layers)
	{
		for (auto &obj : layer)
		{
			kernel.print("Deleting objects: " + obj->getId(), INFO);
			delete obj;
			obj = NULL;
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~