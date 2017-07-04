//-----------------------------------------------------------------------------
// main.cpp														 DEMO-Программа
//-----------------------------------------------------------------------------
#include <iostream>
#include <string>
#include "macros.h"
//-----------------------------------------------------------------------------
#define SCREEN_X 1024
#define SCREEN_Y 768
#define SCREEN_M 7
//-----------------------------------------------------------------------------
#ifdef OS_IS_WIN
	#include <windows.h>
	#define RES_PATH std::string("Resources/")
	#define APP_ICON "icon.png"
#else
	#ifdef DEBUG
		#define RES_PATH std::string("Resources/")
	#else
		#include "Modules/pathfinder.hpp"
		#define RES_PATH findPath()
	#endif
	#define APP_ICON "icon-mac.png"
#endif
//-----------------------------------------------------------------------------
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Modules/engine.hpp"
//-----------------------------------------------------------------------------
int main()
{
	system("chcp 1251 > nul");
	ng::LogStream log;

	log.print("Инициализация окна", 3);

	sf::ContextSettings setting;
	setting.antialiasingLevel = 8;
	sf::VideoMode videoMode(SCREEN_X, SCREEN_Y);
	sf::RenderWindow window(videoMode, "NOVEL FOX ENGINE", SCREEN_M, setting);
	window.setFramerateLimit(30);

	//[XML ЗАГРУЗЧИК ИГРОВЫХ РЕСУРСОВ]
	//loadXMLComposer(RES_PATH + "scenario/script.xml");

	//[ИКОНКА][СТАНДАРТ]
	ng::Icon icon(RES_PATH + APP_ICON); //TO DO: Сделать установку ICON из файла настроек
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	//[ШРИФТ][ТЕКСТ][СТАНДАРТ][ДОРАБОТАТЬ]
	ng::Text text("SFML DEMO", sf::Vector2f(SCREEN_X-200, 20), RES_PATH + "font1.ttf"); 
	text.setString("SFML"); //ПОСТ-ИЗМЕНЕНИЕ ПАРАМЕТРА STRING
	
	//[ТЕКСТУРА][СПРАЙТ][СТАНДАРТ]
	ng::Sprite sprite("background", RES_PATH + "texture.png");

	//[GIF-АНИМАЦИЯ][СТАНДАРТ]
	ng::Sprite gif("gif", RES_PATH + "gifFile.png"); //TO DO: Сделать отдельный класс анимации

	//[ЗАГРУЗКА СПРАЙТОВ С ПОМОЩЬЮ XMLLOADER]
	tinyxml2::XMLElement* sp = ng::parseXML(RES_PATH + "scenario/script.xml");
	ng::Sprite slavya1(ng::getSpriteData(sp, RES_PATH));
	sp = ng::getSpriteXMLNode(sp); // Нода1 -> Нода2
	ng::Sprite slavya2(ng::getSpriteData(sp, RES_PATH));

	//[МУЗЫКА][СТАНДАРТ]
	ng::Music music("music", RES_PATH + "music.ogg");
	
	//[ЗВУК][СТАНДАРТ]
	ng::Sound sound("sound", RES_PATH + "sound.ogg");

	log.print("Ресурсы загружены. Возможные ошибки выведены.", 3);

	bool forward = true; // [!]

	while (window.isOpen())
	{
		//Общий пример обновления позиции в текстуре
		int time = ng::globalClock.getMilliSecond();
		if(ng::globalClock.getMilliSecond() > 1000) ng::globalClock.restart(); 
		else
		{
			gif.setTextureRect(sf::IntRect(256 * (time/500), 0, 256, 256));
			auto pos = slavya1.getPosition();
			if (pos.y < -100) forward = true;
			if (pos.y > -80) forward = false;
			(forward) ? slavya1.setPosition(pos.x, pos.y+1) : slavya1.setPosition(pos.x, pos.y-1);
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (((event.type == sf::Event::KeyPressed) && 
				(event.key.code == sf::Keyboard::Escape)) || event.type == sf::Event::Closed)
				window.close();
			if ((music.getStatus() != sf::Music::Playing) && window.isOpen()) 
				music.play();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
				sound.play();
		}

		window.pushGLStates();
		window.clear();
		window.draw(sprite);
		window.draw(slavya1);
		window.draw(slavya2);
		window.draw(text);
		window.draw(gif);
		window.popGLStates();
		window.display();
	}

	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------