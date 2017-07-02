//-----------------------------------------------------------------------------
// main.cpp														 DEMO-Программа
//-----------------------------------------------------------------------------
#include <iostream>
#include <string>
#include "macros.h"
//-----------------------------------------------------------------------------
#ifdef OS_IS_WIN
	#include <windows.h>
	//#define SCREEN_X GetSystemMetrics(SM_CXSCREEN)
	//#define SCREEN_Y GetSystemMetrics(SM_CYSCREEN)
	#define SCREEN_X 1024
	#define SCREEN_Y 768
	#define SCREEN_M 7
	#define RES_PATH std::string("Resources/")
#else
	#define SCREEN_X 1024
	#define SCREEN_Y 768
	#define SCREEN_M 7
	#ifdef DEBUG
		#define RES_PATH std::string("Resources/")
	#else
		#include "Modules/pathfinder.hpp"
		#define RES_PATH findPath()
	#endif
#endif
//-----------------------------------------------------------------------------
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Modules/loadRes.hpp"
//-----------------------------------------------------------------------------
int main()
{
	sf::ContextSettings setting;
	setting.antialiasingLevel = 8;
	sf::VideoMode videoMode(SCREEN_X, SCREEN_Y);
	sf::RenderWindow window(videoMode, "Three Minutes", SCREEN_M, setting);
	window.setFramerateLimit(30);
	//[XML ЗАГРУЗЧИК ИГРОВЫХ РЕСУРСОВ]
	loadXMLComposer(RES_PATH + "scenario/script.xml");
	//[ВРЕМЯ]
	sf::Clock clock;
	//[ШРИФТ][ТЕКСТ]
	sf::Font *font = new sf::Font;
	font->loadFromFile(RES_PATH + "font1.ttf");
	sf::Text *text = new sf::Text("SFML demo", *font);
	text->setFillColor(sf::Color::Black);
	text->setPosition(sf::Vector2f(SCREEN_X-200, 20));
	//[ТЕКСТУРА][СПРАЙТ]
	sf::Texture texture;
	texture.loadFromFile(RES_PATH + "texture.png");
	sf::Sprite sprite(texture);
	//[GIF-АНИМАЦИЯ][СТАНДАРТНАЯ ЗАГРУЗКА ТЕКСТУРЫ]
	sf::Texture gifT;
	gifT.loadFromFile(RES_PATH + "gifFile.png");
	sf::Sprite gif(gifT);
	//[МУЗЫКА]
	sf::Music music;
	music.openFromFile(RES_PATH + "music.ogg");
	music.setVolume(40);
	//[ЗВУК]
	sf::SoundBuffer buffer;
	buffer.loadFromFile(RES_PATH + "sound.ogg"); 
	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.setVolume(40);

	while (window.isOpen())
	{
		//Общий пример обновления позиции в текстуре
		int time = clock.getElapsedTime().asMilliseconds();
		if(clock.getElapsedTime().asMilliseconds() > 1000) clock.restart(); else
		gif.setTextureRect(sf::IntRect(256 * (time/500), 0, 256, 256));

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
		window.draw(*text);
		window.draw(gif);
		window.popGLStates();
		window.display();
	}
	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------