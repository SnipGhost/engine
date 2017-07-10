//-----------------------------------------------------------------------------
// main.cpp														 DEMO-Программа
//-----------------------------------------------------------------------------
#include "Engine/engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
int main()
{
	std::map<std::string, Font*> fonts;
	XMLNode fontElem = parseXML("FONT");
	Font *font;
	while (fontElem != NULL)
	{
		FontData fd = getFontData(fontElem);
		font = new Font(fd);
		kernel.print(fd.src);
		fonts[fd.id] = font;
		fontElem = getNextXMLNode(fontElem, "FONT");
	}

	kernel.print("0");

	// [ТЕКСТ]
	XMLNode tElement = parseXML("TEXT");
	Text text1(getTextData(tElement, fonts));
	tElement = getNextXMLNode(tElement, "TEXT"); // Нода1 -> Нода2
	Text text2(getTextData(tElement, fonts));

	kernel.print("1");

	// [GIF-АНИМАЦИЯ]
	XMLNode asElement = parseXML("GIF");
	AnimateSprite gif1(getAnimateSpriteData(asElement));
	kernel.print(gif1, INFO);
	asElement = getNextXMLNode(asElement, "GIF"); // Нода1 -> Нода2
	AnimateSprite gif2(getAnimateSpriteData(asElement));
	kernel.print(gif2, INFO);

	kernel.print("2");

	// [MAP]
	std::map<std::string, ng::Displayable*> objects;
	typedef std::map<std::string, ng::Displayable*>::iterator Iter;

	// [СПРАЙТ]
	XMLNode spElement = parseXML("SPRITE");
	Sprite *sprite = NULL;
	while (spElement != NULL)
	{
		SpriteData data = getSpriteData(spElement);
		sprite = new Sprite(data);
		kernel.print(*sprite, INFO);
		objects[data.id] = sprite;
		spElement = getNextXMLNode(spElement, "SPRITE");
	}

	// [ВИДЕО]
	XMLNode vElement = parseXML("VIDEO");
	Video video(getVideoData(vElement));
	kernel.print(video, INFO);

	video.play();
	
	objects["gif_1"] = &gif1;
	objects["gif_2"] = &gif2;
	objects["video"] = &video;
	objects["text1"] = &text1;
	objects["text2"] = &text2;

	// [МУЗЫКА]
	XMLNode mElement = parseXML("MUSIC");
	Music music(getMusicData(mElement));
	/*kernel.print(music, INFO);*/ //!!!
	music.play();

	// [ЗВУК]
	XMLNode sElement = parseXML("SOUND");
	Sound sound(getSoundData(sElement));
	kernel.print(sound, INFO);

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
			delay(500);
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