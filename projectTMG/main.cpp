//-----------------------------------------------------------------------------
// main.cpp
//                                                               DEMO-Программа
//-----------------------------------------------------------------------------
#include <iostream>
#include <string>
#include "macros.h"
//-----------------------------------------------------------------------------
#ifdef OS_IS_WIN
    #include <windows.h>
    #define SCREEN_X GetSystemMetrics(SM_CXSCREEN)
    #define SCREEN_Y GetSystemMetrics(SM_CYSCREEN)
    #define SCREEN_M 8
    #define RES_PATH std::string("Resources/")
#else
    #define SCREEN_X 1024
    #define SCREEN_Y 768
    #define SCREEN_M 7
    #ifdef DEBUG
        #define RES_PATH std::string("Resources/")
    #else
        #include "modules/pathfinder.hpp"
        #define RES_PATH findPath()
    #endif
#endif
//-----------------------------------------------------------------------------
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "modules/loadRes.hpp"
//-----------------------------------------------------------------------------
int main()
{
    std::cout << RES_PATH << std::endl;
	sf::ContextSettings setting;
	setting.antialiasingLevel = 8; // Сглаживаниe
    sf::VideoMode videoMode(SCREEN_X, SCREEN_Y);
	sf::RenderWindow window(videoMode, "Three Minutes", SCREEN_M, setting);
    window.setFramerateLimit(30);
	//[ЗАГРУЗЧИК ИГРОВЫХ РЕСУРСОВ]
	//loadXMLResources(RES_PATH + "scenario/script.xml");
	//[ШРИФТ]
	sf::Font *font = new sf::Font;
	font->loadFromFile(RES_PATH + "font1.ttf");
	//[ТЕКСТ]
	sf::Text *text = new sf::Text("SFML demo", *font);
	text->setFillColor(sf::Color::Black);
    text->setPosition(sf::Vector2f(SCREEN_X-200, 20));
	//[ТЕКСТУРА]
	sf::Texture texture;
	texture.loadFromFile(RES_PATH + "texture.png");
	//[СПРАЙТ]
	sf::Sprite sprite(texture);
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
		sf::Event event;
		while (window.pollEvent(event))
		{
			if ((event.type == sf::Event::KeyPressed) && 
                (event.key.code == sf::Keyboard::Escape))
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
		window.popGLStates();
		window.display();
	}
	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------