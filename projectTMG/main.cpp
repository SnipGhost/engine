#include "macros.h"

#ifdef OS_IS_WIN
    #include <windows.h>
    #define SCREEN_Y GetSystemMetrics(SM_CYSCREEN)
    #define SCREEN_X GetSystemMetrics(SM_CXSCREEN)
    #define SCREEN_M 8
#else
    #define SCREEN_Y 600
    #define SCREEN_X 800
    #define SCREEN_M 7
#endif

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

int main()
{
	int style = SCREEN_M; //Fullscreen
	sf::ContextSettings setting;
	setting.antialiasingLevel = 8; //Сглаживание. Производительность?!
	sf::RenderWindow window(sf::VideoMode(SCREEN_X, SCREEN_Y), "Three Minutes", style, setting);
	//[УЗНАЁМ О РАЗМЕРАХ ОКНА]
	//sf::Vector2u size = window.getSize();
	//unsigned int width = size.x;
	//unsigned int height = size.y;
	//[ШРИФТ]
	sf::Font *font = new sf::Font;
	font->loadFromFile("resources/font1.ttf");
	//[ТЕКСТ]
	sf::Text *text = new sf::Text("SFML demo", *font);
	text->setColor(sf::Color::Black);
	//[ТЕКСТУРА]
	sf::Texture texture;
	texture.loadFromFile("resources/texture.png");
	//[СПРАЙТ]
	sf::Sprite sprite(texture);
	//[МУЗЫКА]
	sf::Music music;
	music.openFromFile("resources/music.ogg");
	music.setVolume(40);
	//[ЗВУК]
	sf::SoundBuffer buffer;
	buffer.loadFromFile("resources/sound.ogg"); 
	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.setVolume(40);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				window.close();
			if (music.getStatus() != sf::Music::Playing) if(window.isOpen()) music.play();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) sound.play();
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