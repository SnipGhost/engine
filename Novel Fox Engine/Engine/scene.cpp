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
	tEvent = -1;
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
	std::string line = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
	kernel.print(line + "[Start loading resources]", INFO);
	const char *sceneId = scene->Attribute("id");
	if (sceneId)
		kernel.print("\t\t\t\t\t\t\t\t[Scene id: " + std::string(sceneId) + "]", INFO);
	else
		kernel.print("\t\t\t\t\t\t\t\t\t\t  [Event]", INFO);
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
			ResData data = getData(node);
			switch (i)
			{
			case 0: // [SPRITE]
			{
				if (objects[data.id])
				{	
					if (GETBIT(data.bitMask, _visible)) objects[data.id]->visible = data.visible;
					objects[data.id]->edit(data);
				}
				else
				{
					objects[data.id] = new Sprite(data);
					layers[data.layer + MAX_LAYER].push_back(objects[data.id]);
					kernel.print(objects[data.id], INFO);
				}
				break;
			}
			case 1: // [ANIMATESPRITE]
			{
				if (objects[data.id])
				{	
					if (GETBIT(data.bitMask, _visible)) objects[data.id]->visible = data.visible;
					objects[data.id]->edit(data);
				}
				else
				{
					objects[data.id] = new AnimateSprite(data);
					layers[data.layer + MAX_LAYER].push_back(objects[data.id]);
					kernel.print(objects[data.id], INFO);
				}
				break;
			}
			case 2: // [VIDEO]
			{
				if (objects[data.id])
				{
					if (GETBIT(data.bitMask, _visible)) objects[data.id]->visible = data.visible;
					objects[data.id]->edit(data);
				}
				else
				{
					Video *video = new Video(data);
					video->play();
					videos[data.id] = video;
					objects[data.id] = video;
					layers[data.layer + MAX_LAYER].push_back(objects[data.id]);
					kernel.print(video, INFO);
				}
				break;
			}
			case 3: // [TEXT]
			{
				if (objects[data.id])
				{
					if (GETBIT(data.bitMask, _visible)) objects[data.id]->visible = data.visible;
					objects[data.id]->edit(data);
				}
				else
				{
					//Рассчёт нужного количества конструкторов Text: Имя, 1-я строчка, 2-я строчка и т.д.
					objects[data.id] = new Text(data);
					layers[data.layer + MAX_LAYER].push_back(objects[data.id]);
					kernel.print(objects[data.id], INFO);
				}
				break;
			}
			case 4: // [MUSIC]
			{
				if (kernel.music[data.id])
				{
					if (data.command == "stop") // Остановка и удаление
					{
						delete kernel.music[data.id];
						kernel.music[data.id] = nullptr;
					}
					if (data.command == "pause") // Остановка на паузу
					{
						kernel.music[data.id]->playable = false;
						kernel.music[data.id]->pause();
					}
					if (data.command == "play") // Остановка на паузу
					{
						kernel.music[data.id]->playable = true;
						kernel.music[data.id]->play();
					}

					//if (data.command == "slowstop") // Плавная остатовка и удаление
					//	kernel.music[data.id]->state = "slowstop";
					//if (data.command == "slowpause") // Плавная остановка на паузу
					//	kernel.music[data.id]->state = "slowpause";
					//if (data.command == "slowplay") // Плавное включение
					//	kernel.music[data.id]->state = "slowstart";


					if (data.command == "edit")
					{
						//if (data.volume) kernel.music[data.id]->setVolume(data.volume);
						//(data.loop) ? kernel.music[data.id]->setLoop(true) : kernel.music[data.id]->setLoop(false);
						//(data.visible) ? kernel.music[data.id]->playable = true : kernel.music[data.id]->playable = false; //Нужна ли?
					}
				}											
				else
				{
					Music *mus = new Music(data);
					mus->play();
					kernel.music[data.id] = mus;
					kernel.print(mus, INFO);
				}
				break;
			}
			case 5: // [SOUND]
			{
				if (sounds[data.id]) // Map со звуками очищается при delete scene и менять звук можно в момент EVENT, для новой scene придётся создавать новый звук
				{	
					if (data.command == "edit")
					{
						//if (data.volume) sounds[data.id]->setVolume(data.volume);
					}
				}
				else
				{
					sounds[data.id] = new Sound(data);
					kernel.print(sounds[data.id], INFO);
				}
				break;
			}
			}
			node = kernel.getNextXMLNode(node, TAGS[i]);
		}
	}
	kernel.print(line + "~~~~~~~[Resources loaded]", INFO);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Read event
bool Scene::doEvent(XMLNode scene)
{
	if (firstEvent)
	{
		eventNode = scene->FirstChildElement("EVENT"); 
		if (eventNode)
		{
			loadScene(eventNode);
			const char *timeEvent = eventNode->Attribute("time");  //Добавить переключение по другим причинам
			if (timeEvent) tEvent = std::atoi(timeEvent);
			saveTTEvent = kernel.globalClock.getMilliSecond();
		}
		else
			return 1;
		firstEvent = false;
	} 
	else
	{
		eventNode = eventNode->NextSiblingElement("EVENT");
		if (eventNode)
		{
			loadScene(eventNode);
			const char *timeEvent = eventNode->Attribute("time");  //Добавить переключение по другим причинам
			if (timeEvent) tEvent = std::atoi(timeEvent);
			saveTTEvent = kernel.globalClock.getMilliSecond();
		}
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
		if (video.second && !kernel.event.isVideoPlay(*video.second) && 
			video.second->visible)
			video.second->play();
	for (auto &tempo : kernel.music)
		if (tempo.second && !kernel.event.isMusicPlay(*tempo.second) && 
			tempo.second->playable && tempo.second->getLoop() == true) //[?]
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
	for (auto &tempo : kernel.music)
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
			delete obj;
			obj = NULL;
		}
	}
	for (auto &sound : sounds) 
		if (sound.second)
			delete sound.second;
	//for (auto &tempo : music) // Музыку удаляет сам дальнейший разработчик из XML сценария
	//	if (tempo.second)
	//		delete tempo.second;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~