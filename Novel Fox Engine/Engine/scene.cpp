//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// engine.cpp                         Реализации ядра и вспомогательных функций
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Scene::Scene(XMLNode scene)
{
	firstEvent = true;
	loadScene(scene);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Scene::~Scene()
{
	clear();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Scene::loadScene(XMLNode scene)
{
	const size_t COUNT = 6;
	const char *TAGS[COUNT] = {
		"SPRITE", "ANIMATION", "VIDEO", "TEXT", "MUSIC", "SOUND"
	};
	XMLNode node = NULL;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (size_t i = 0; i < COUNT; ++i)
	{
		node = kernel.parseXML(scene, TAGS[i]);
		while (node != NULL)
		{
			Data data = getData(node);
			switch (i)
			{
			case 0:
			{
				objects[data.id] = new Sprite(data);
				layers[data.layer + MAX_LAYER].push_back(objects[data.id]);
				kernel.print(objects[data.id], INFO);
				break;
			}
			case 1:
			{
				objects[data.id] = new AnimateSprite(data);
				layers[data.layer + MAX_LAYER].push_back(objects[data.id]);
				kernel.print(objects[data.id], INFO);
				break;
			}
			case 2:
			{
				Video *video = new Video(data);
				video->play();
				videos[data.id] = video;
				objects[data.id] = video;
				layers[data.layer + MAX_LAYER].push_back(objects[data.id]);
				kernel.print(video, INFO);
				break;
			}
			case 3:
			{
				objects[data.id] = new Text(data);
				layers[data.layer + MAX_LAYER].push_back(objects[data.id]);
				kernel.print(objects[data.id], INFO);
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
// Read event
bool Scene::doEvent(XMLNode scene)
{
	if (firstEvent)
	{
		eventNode = scene->FirstChildElement("EVENT"); 
		if (eventNode)
			loadScene(eventNode);
		else
			return 1;
		firstEvent = false;
	} 
	else
	{
		eventNode = eventNode->NextSiblingElement("EVENT");
		if (eventNode)
			loadScene(eventNode);
		else
			return 1;
	}
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Jump check
bool Scene::jump(XMLNode node)
{
	if (node->FirstChildElement("JUMP")) return 1;
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Включение любых проигрывающихся ресурсов
void Scene::startMedia()
{
	for (auto &video : videos)
		if (!kernel.event.isVideoPlay(*video.second))
			video.second->play();
	for (auto &tempo : music)
		if (!kernel.event.isMusicPlay(*tempo.second) && tempo.second->playable)
			tempo.second->play();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Остановка любых проигрывающихся ресурсов
void Scene::stopMedia()
{
	for (auto &sound : sounds)
		if (sound.second)
			sound.second->stop();
	for (auto &video : videos)
		if (video.second)
			video.second->setPause();
	for (auto &tempo : music)
		if (tempo.second)
			tempo.second->setPause();
	delay(FOCUS_DELAY);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Отрисовка всех объектов
void Scene::displayAll()
{
	for (auto &layer : layers)
	{
		for (auto &obj : layer)
		{
			if (obj->visible)
			{
				obj->setLayerMotion();
				obj->display();
			}
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Удаление всех отрисованных объектов
void Scene::clear()
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
	for (auto &sound : sounds) 
		if (sound.second)
			delete sound.second;
	for (auto &tempo : music) 
		if (tempo.second)
			delete tempo.second;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~