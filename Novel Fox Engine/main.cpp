//-----------------------------------------------------------------------------
// main.cpp														 DEMO-Программа
//-----------------------------------------------------------------------------
#include "Engine/engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
int main()
{
	//[ШРИФТ][ТЕКСТ][СТАНДАРТ][ДОРАБОТАТЬ]
	unsigned int screen_x = kernel.window->getSize().x;
	Text text(L"Хуманизацию подвезли", (float)screen_x-250, 50, 20, RES_PATH); 

	//[ТЕКСТУРА][СПРАЙТ][СТАНДАРТ]
	Sprite background(RES_PATH + "texture.png");

	//[GIF-АНИМАЦИЯ][СТАНДАРТ]
	AnimateSprite gif(RES_PATH + "gifFile.png");   // [12 КАДРОВ GIF]
	gif.setAnimation(256);
	gif.setPosition(50, 300);
	AnimateSprite gif1(RES_PATH + "gifFile1.png"); // [ 6 КАДРОВ GIF]
	gif1.setAnimation(256);
	gif1.setPosition(50, 500);

	//[ЗАГРУЗКА СПРАЙТОВ С ПОМОЩЬЮ XMLLOADER]
	tinyxml2::XMLElement* sp = parseXML("SPRITE");
	Sprite maya1(getSpriteData(sp, RES_PATH));

	sp = getNextXMLNode(sp, "SPRITE"); // Нода1 -> Нода2
	Sprite maya2(getSpriteData(sp, RES_PATH));

	//std::map <sf::Sprite, std::string> mapping; //TO DO

	//[МУЗЫКА][СТАНДАРТ]
	Music music(RES_PATH + "music.ogg");
	music.play();

	//[ЗВУК][СТАНДАРТ]
	Sound sound(RES_PATH + "sound.ogg");

	//[ВИДЕО][СТАНДАРТ]
	Video video(RES_PATH + "video.ogv", 711, 400, 0, 0, 0);
	video.play();

	kernel.print("Ресурсы загружены. Возможные ошибки выведены.", NORM);

	while (kernel.window->isOpen())
	{
		Event event;
		while (kernel.window->pollEvent(event))
		{
			if (event.isKeyboardKey(event.keyboard.Escape) ||
				event.isWinClosed()) kernel.window->close();
			if (event.isMouseClickKey(sf::Mouse::Left)) sound.play();
			if (!event.isMusicPlay(music)) music.play();
		}
		if (event.isMouseKey(sf::Mouse::Right)) music.stopMusic();

		//if (!kernel.window->hasFocus()) //АДЕКВАТСТВА БЫ ПОБОЛЬШЕ
		//{
		//	music.pause();
		//	sound.pause();
		//	continue;
		//}

		kernel.window->pushGLStates();
		kernel.window->clear();

		background.display();
		maya1.display();
		maya2.display();
		text.display();
		gif.display();
		gif1.display();
		video.display();

		kernel.window->popGLStates();
		kernel.window->display();
	}

	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------