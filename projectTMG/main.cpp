#include <windows.h> //[WINDOWS]-MAC
#include <SFML/Graphics.hpp>

#define SCREEN_Y GetSystemMetrics(SM_CYSCREEN) //[WINDOWS]-MAC
#define SCREEN_X GetSystemMetrics(SM_CXSCREEN) //[WINDOWS]-MAC

int main()
{
	int style = 8; //Fullscreen
	sf::ContextSettings setting;
	setting.antialiasingLevel = 8; //Сглаживание. Производительность?!
	sf::RenderWindow window(sf::VideoMode(SCREEN_X, SCREEN_Y), "Three Minutes", style, setting);
	//[ШРИФТ]
	sf::Font *font = new sf::Font;
	font->loadFromFile("resources/font1.ttf");
	//[ТЕКСТ]
	sf::Text *text = new sf::Text("SFML / OpenGL demo", *font);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				window.close();
		}
		window.pushGLStates();
		window.clear();
		window.draw(*text);
		window.popGLStates();
		window.display();
	}
	return EXIT_SUCCESS;
}