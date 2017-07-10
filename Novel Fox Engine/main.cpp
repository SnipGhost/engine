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
	XMLNode node = NULL;
	std::map<std::string, ng::Displayable*> objects;
	typedef std::map<std::string, ng::Displayable*>::iterator Iter;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// [ШРИФТЫ]
	node = parseXML("FONT");
	while (node != NULL)
	{
		FontData data = getFontData(node);
		Font *font = new Font(data);
		fonts[data.id] = font;
		kernel.print("Loaded font: " + data.src, INFO);
		node = getNextXMLNode(node, "FONT");
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// [ТЕКСТ]
	node = parseXML("TEXT");
	while (node != NULL)
	{
		TextData data = getTextData(node, fonts);
		Text *text = new Text(data);
		objects[data.id] = text;
		kernel.print(text, INFO);
		node = getNextXMLNode(node, "TEXT");
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// [АНИМАЦИЯ]
	node = parseXML("GIF");
	while (node != NULL)
	{
		AnimateSpriteData data = getAnimateSpriteData(node);
		AnimateSprite *asprite = new AnimateSprite(data);
		objects[data.id] = asprite;
		kernel.print(asprite, INFO);
		node = getNextXMLNode(node, "GIF");
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// [СПРАЙТ]
	node = parseXML("SPRITE");
	while (node != NULL)
	{
		SpriteData data = getSpriteData(node);
		Sprite *sprite = new Sprite(data);
		kernel.print(sprite, INFO);
		objects[data.id] = sprite;
		node = getNextXMLNode(node, "SPRITE");
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// [ВИДЕО] ТОЛЬКО ОДНО, ИЗ-ЗА 93 101 СТРОК
	node = parseXML("VIDEO");
	Video *video = NULL;
	if (node != NULL)
	{
		VideoData data = getVideoData(node);
		video = new Video(data);
		video->play();
		objects[data.id] = video;
		kernel.print(video, INFO);
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// [МУЗЫКА]
	XMLNode mElement = parseXML("MUSIC");
	Music music(getMusicData(mElement));
	kernel.print(&music, INFO); // Открыл конструктор копий, но указатель лучше
	music.play();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// [ЗВУК]
	XMLNode sElement = parseXML("SOUND");
	Sound sound(getSoundData(sElement));
	kernel.print(&sound, INFO);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	kernel.print("Resources loaded.", NORM);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	while (kernel.window->isOpen())
	{
		Event event;
		while (kernel.window->pollEvent(event))
		{
			if (event.isKeyboardKey(event.keyboard.Escape) ||
				event.isWinClosed()) kernel.window->close();
			if (event.isMouseClickKey(sf::Mouse::Left)) sound.play();
			if (!event.isMusicPlay(music)) music.play();
			if (!event.isVideoPlay(*video)) video->play();
		}
		if (event.isMouseKey(sf::Mouse::Right)) music.setStop();

		if (!kernel.window->hasFocus()) // УЖАСНАЯ ИДЕЯ БЕЗ SLEEP
		{
			music.setPause();
			sound.stop();
			video->setPause();
			delay(FOCUS_DELAY);
			continue;
		}

		startDisplay();

		for (Iter it = objects.begin(); it != objects.end(); ++it)
			it->second->display();

		endDisplay();
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (Iter it = objects.begin(); it != objects.end(); ++it)
	{
		kernel.print("Deleting: " + it->first, INFO);
		delete it->second;
		it->second = NULL;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------