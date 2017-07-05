//-----------------------------------------------------------------------------
// main.cpp														 DEMO-Программа
//-----------------------------------------------------------------------------
#define SCREEN_X 1024
#define SCREEN_Y 768
#define SCREEN_M 7
//-----------------------------------------------------------------------------
#include "Engine/engine.hpp"
//-----------------------------------------------------------------------------
int main()
{
	system("chcp 1251 > nul");

	ng::log.print("Инициализация окна", 3);

	sf::ContextSettings setting;
	setting.antialiasingLevel = 8;
	sf::VideoMode videoMode(SCREEN_X, SCREEN_Y);
	sf::RenderWindow window(videoMode, "NOVEL FOX ENGINE", SCREEN_M, setting);
	window.setFramerateLimit(30);

	//[XML ЗАГРУЗЧИК ИГРОВЫХ РЕСУРСОВ]
	//loadXMLComposer(RES_PATH + "scenario/script.xml");

	//[ИКОНКА][СТАНДАРТ]
	ng::Icon icon(RES_PATH + APP_ICON);
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	//[ШРИФТ][ТЕКСТ][СТАНДАРТ][ДОРАБОТАТЬ]
	ng::Text text(L"Привет", SCREEN_X-200, 50, 20, RES_PATH); 
	
	//[ТЕКСТУРА][СПРАЙТ][СТАНДАРТ]
	ng::Sprite background("background", RES_PATH + "texture.png");

	//[GIF-АНИМАЦИЯ][СТАНДАРТ]
	ng::Sprite gif("gif", RES_PATH + "gifFile.png");    //[12 КАДРОВ GIF]
	gif.setPosition(50, 300);
	ng::Sprite gif1("gif1", RES_PATH + "gifFile1.png"); //[6 КАДРОВ GIF]
	gif1.setPosition(50, 500);

	//[ЗАГРУЗКА СПРАЙТОВ С ПОМОЩЬЮ XMLLOADER]
	tinyxml2::XMLElement* sp = ng::parseXML(RES_PATH + "scenario/script.xml");
	ng::Sprite slavya1(ng::getSpriteData(sp, RES_PATH));
	sp = ng::getSpriteXMLNode(sp); // Нода1 -> Нода2
	ng::Sprite slavya2(ng::getSpriteData(sp, RES_PATH));

	std::map <sf::Sprite, std::string> mapping; //TO DO

	//[МУЗЫКА][СТАНДАРТ]
	ng::Music music(RES_PATH + "music.ogg");
	
	//[ЗВУК][СТАНДАРТ]
	ng::Sound sound(RES_PATH + "sound.ogg");

	ng::log.print("Ресурсы загружены. Возможные ошибки выведены.", 3);

	while (window.isOpen())
	{

		gif.setAnimation(ng::globalClock.getMilliSecond());
		gif1.setAnimation(ng::globalClock.getMilliSecond());

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
		window.draw(background);
		window.draw(slavya1);
		window.draw(slavya2);
		window.draw(text);
		window.draw(gif);
		window.draw(gif1);
		window.popGLStates();
		window.display();
	}

	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------