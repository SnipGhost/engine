//-----------------------------------------------------------------------------
// main.cpp														 DEMO-Программа
//-----------------------------------------------------------------------------
#include "Engine/engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
int main()
{
	typedef tinyxml2::XMLElement Elem;

	// [ТЕКСТ][ДОРАБОТАТЬ][ЛИТЕРАЛ!!!]
	Elem* tElement = parseXML("TEXT");
	Text text1(getTextData(tElement, RES_PATH));
	tElement = getNextXMLNode(tElement, "TEXT"); // Нода1 -> Нода2
	Text text2(getTextData(tElement, RES_PATH));

	// [GIF-АНИМАЦИЯ]
	Elem* asElement = parseXML("GIF");
	AnimateSprite gif1(getAnimateSpriteData(asElement, RES_PATH));
	asElement = getNextXMLNode(asElement, "GIF"); // Нода1 -> Нода2
	AnimateSprite gif2(getAnimateSpriteData(asElement, RES_PATH));

	// [СПРАЙТ]
	Elem* spElement = parseXML("SPRITE");
	Sprite background(getSpriteData(spElement, RES_PATH));
	spElement = getNextXMLNode(spElement, "SPRITE"); // Нода1 -> Нода2
	Sprite maya1(getSpriteData(spElement, RES_PATH));
	spElement = getNextXMLNode(spElement, "SPRITE"); // Нода2 -> Нода3
	Sprite maya2(getSpriteData(spElement, RES_PATH));

	// [ВИДЕО]
	Elem* vElement = parseXML("VIDEO");
	Video video(getVideoData(vElement, RES_PATH));
	video.play();

	// [MAP]
	std::map<std::string, ng::Displayable*> objects;
	typedef std::map<std::string, ng::Displayable*>::iterator Iter;

	objects["bg"] = &background;
	objects["maya1"] = &maya1;
	objects["maya2"] = &maya2;
	objects["gif_1"] = &gif1;
	objects["gif_2"] = &gif2;
	objects["video"] = &video;
	objects["text1"] = &text1;
	objects["text2"] = &text2;

	// [МУЗЫКА]
	Elem* mElement = parseXML("MUSIC");
	Music music(getMusicData(mElement, RES_PATH));
	music.play();

	// [ЗВУК]
	Elem* sElement = parseXML("SOUND");
	Sound sound(getSoundData(sElement, RES_PATH));

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
			if (!event.isVideoPlay(video)) video.play();
		}
		if (event.isMouseKey(sf::Mouse::Right)) music.setStop();

		if (!kernel.window->hasFocus())
		{
			music.setPause();
			sound.stop();
			video.setPause();
			continue;
		}

		startDisplay();
		for (Iter it = objects.begin(); it != objects.end(); ++it)
		{
			it->second->display();
		}
		endDisplay();
	}

	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------