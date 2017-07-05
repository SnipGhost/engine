//-----------------------------------------------------------------------------
// main.cpp														 DEMO-Программа
//-----------------------------------------------------------------------------
#include "Engine/engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
int main()
{
	kernel.print("Окно инициализированно", NORM);

	//[ШРИФТ][ТЕКСТ][СТАНДАРТ][ДОРАБОТАТЬ]
	Text text(L"Привет", SCREEN_X-200, 50, 20, RES_PATH); 
	
	//[ТЕКСТУРА][СПРАЙТ][СТАНДАРТ]
	Sprite background(RES_PATH + "texture.png");

	//[GIF-АНИМАЦИЯ][СТАНДАРТ]
	AnimateSprite gif(RES_PATH + "gifFile.png");   // [12 КАДРОВ GIF]
	gif.setAnimation(256, 256, 40);
	gif.setPosition(50, 300);
	AnimateSprite gif1(RES_PATH + "gifFile1.png"); // [ 6 КАДРОВ GIF]
	gif1.setAnimation(256, 256, 40);
	gif1.setPosition(50, 500);

	//[ЗАГРУЗКА СПРАЙТОВ С ПОМОЩЬЮ XMLLOADER]
	tinyxml2::XMLElement* sp = parseXML();
	Sprite slavya1(getSpriteData(sp, RES_PATH));
	
	sp = getSpriteXMLNode(sp); // Нода1 -> Нода2
	Sprite slavya2(getSpriteData(sp, RES_PATH));

	//std::map <sf::Sprite, std::string> mapping; //TO DO

	//[МУЗЫКА][СТАНДАРТ]
	Music music(RES_PATH + "music.ogg");
	
	//[ЗВУК][СТАНДАРТ]
	Sound sound(RES_PATH + "sound.ogg");

	kernel.print("Ресурсы загружены. Возможные ошибки выведены.", 3);

	while (kernel.window->isOpen())
	{
		sf::Event event;
		while (kernel.window->pollEvent(event))
		{
			if (((event.type == sf::Event::KeyPressed) && 
				(event.key.code == sf::Keyboard::Escape)) || event.type == sf::Event::Closed)
				kernel.window->close();
			if ((music.getStatus() != sf::Music::Playing) && kernel.window->isOpen()) 
				music.play();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
				sound.play();
		}

		kernel.window->pushGLStates();
		kernel.window->clear();

		background.draw();
		slavya1.draw();
		slavya2.draw();

		text.draw();

		gif.draw();
		gif1.draw();

		kernel.window->popGLStates();
		kernel.window->display();
	}

	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------