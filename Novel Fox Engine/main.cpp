//-----------------------------------------------------------------------------
// main.cpp														 DEMO-Программа
//-----------------------------------------------------------------------------
#include "Engine/engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
int main()
{
	kernel.print("0");

	// [ТЕКСТ][ДОРАБОТАТЬ][ЛИТЕРАЛ!!!]
	XMLNode tElement = parseXML("TEXT");
	Text text1(getTextData(tElement, RES_PATH));
	tElement = getNextXMLNode(tElement, "TEXT"); // Нода1 -> Нода2
	Text text2(getTextData(tElement, RES_PATH));

	kernel.print("1");

	// [GIF-АНИМАЦИЯ]
	XMLNode asElement = parseXML("GIF");
	AnimateSprite gif1(getAnimateSpriteData(asElement, RES_PATH));
	kernel.print(gif1, INFO);
	asElement = getNextXMLNode(asElement, "GIF"); // Нода1 -> Нода2
	AnimateSprite gif2(getAnimateSpriteData(asElement, RES_PATH));
	kernel.print(gif2, INFO);

	kernel.print("2");

	// [MAP]
	std::map<std::string, ng::Displayable*> objects;
	typedef std::map<std::string, ng::Displayable*>::iterator Iter;

	kernel.print("3");

	// [СПРАЙТ]
	XMLNode spElement = parseXML("SPRITE");
	Sprite *sprite = NULL;
	while (spElement != NULL)
	{
		SpriteData data = getSpriteData(spElement, RES_PATH);
		sprite = new Sprite(data);
		kernel.print(*sprite, INFO);
		objects[data.id] = sprite;
		spElement = getNextXMLNode(spElement, "SPRITE");
	}

	kernel.print("4");

	// [ВИДЕО]
	XMLNode vElement = parseXML("VIDEO");
	Video video(getVideoData(vElement, RES_PATH));
	kernel.print(video, INFO);
	video.play();

	kernel.print("5");
	
	objects["gif_1"] = &gif1;
	objects["gif_2"] = &gif2;
	objects["video"] = &video;
	objects["text1"] = &text1;
	objects["text2"] = &text2;

	kernel.print("6");

	// [МУЗЫКА]
	XMLNode mElement = parseXML("MUSIC");
	Music music(getMusicData(mElement, RES_PATH));
	music.play();

	kernel.print("7");

	// [ЗВУК]
	XMLNode sElement = parseXML("SOUND");
	Sound sound(getSoundData(sElement, RES_PATH));

	kernel.print("Resources loaded.", NORM);

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

		if (!kernel.window->hasFocus()) // УЖАСНАЯ ИДЕЯ БЕЗ SLEEP
		{
			music.setPause();
			sound.stop();
			video.setPause();
			delay(100);
			continue;
		}

		startDisplay();

		for (Iter it = objects.begin(); it != objects.end(); ++it)
			it->second->display();

		endDisplay();
	}

	// DELETE ALL OBJECTS

	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------