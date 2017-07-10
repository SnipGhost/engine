//-----------------------------------------------------------------------------
// main.cpp														 DEMO-Программа
//-----------------------------------------------------------------------------
#include "Engine/engine.hpp"
#include "Modules/primitiveGui.h"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
int main()
{
	std::map<std::string, Font*> fonts;
	XMLNode node = NULL;
	std::map<std::string, ng::Video*> videos;
	std::map<std::string, ng::Displayable*> objects;
	typedef std::map<std::string, ng::Displayable*>::iterator ObjIt;
	typedef std::map<std::string, ng::Video*>::iterator VidIt;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ШРИФТЫ]
	node = parseXML("FONT");
	while (node != NULL)
	{
		FontData data = getFontData(node);
		Font *font = new Font(data);
		fonts[data.id] = font;
		kernel.print("Loaded font: " + data.src, INFO);
		node = getNextXMLNode(node, "FONT");
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ТЕКСТ]
	node = parseXML("TEXT");
	while (node != NULL)
	{
		TextData data = getTextData(node, fonts);
		Text *text = new Text(data);
		objects[data.id] = text;
		kernel.print(text, INFO);
		node = getNextXMLNode(node, "TEXT");
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[АНИМАЦИЯ]
	node = parseXML("GIF");
	while (node != NULL)
	{
		AnimateSpriteData data = getAnimateSpriteData(node);
		AnimateSprite *asprite = new AnimateSprite(data);
		objects[data.id] = asprite;
		kernel.print(asprite, INFO);
		node = getNextXMLNode(node, "GIF");
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[СПРАЙТ]
	node = parseXML("SPRITE");
	while (node != NULL)
	{
		SpriteData data = getSpriteData(node);
		Sprite *sprite = new Sprite(data);
		kernel.print(sprite, INFO);
		objects[data.id] = sprite;
		node = getNextXMLNode(node, "SPRITE");
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ВИДЕО]
	node = parseXML("VIDEO");
	Video *video = NULL;
	if (node != NULL)
	{
		VideoData data = getVideoData(node);
		video = new Video(data);
		video->play();
		videos[data.id] = video;
		objects[data.id] = video;
		kernel.print(video, INFO);
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[МУЗЫКА]
	XMLNode mElement = parseXML("MUSIC");
	Music music(getMusicData(mElement));
	kernel.print(&music, INFO); // Открыл конструктор копий, но указатель лучше
	music.play();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ЗВУК]
	XMLNode sElement = parseXML("SOUND");
	Sound sound(getSoundData(sElement));
	kernel.print(&sound, INFO);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	kernel.print("Resources loaded.", NORM);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	pProgressBar progressBar(50, 20, 200, 50);
	pSlider slider(50, 100, 200, 40);

	pButton button1(400, 20, 200, 50);
	button1.setText(sf::String("Show/Hide Window 1"));

	pWindow window1(400, 250, 300, 300);
	window1.setWindowTitle(sf::String("Window No. 1"));
	pButton onWindow1Button(20, 20, 200, 40);
	onWindow1Button.setText(sf::String("Change text"));
	pTextEdit onWindow1TextEdit(20, 80, 200, 40);

	window1.addWidget(&onWindow1Button);
	window1.addWidget(&onWindow1TextEdit);

	pWindow window2(50, 250, 250, 300);
	window2.setWindowTitle(sf::String("Window No. 2"));
	pCheckBox onWindow2CheckBox(30, 30, 200, 30);
	onWindow2CheckBox.setText(sf::String("Check me :)"));

	window2.addWidget(&onWindow2CheckBox);

	pGui gui;
	gui.addWidget(&progressBar);
	gui.addWidget(&slider);
	gui.addWidget(&button1);

	gui.addWindow(&window1);
	gui.addWindow(&window2);
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	while (kernel.window->isOpen())
	{
		Event event;
		while (kernel.window->pollEvent(event))
		{
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			gui.update(event, *kernel.window);

			if (onWindow1Button.widgetEvent == pWidget::WidgetClicked)
				onWindow1TextEdit.setText(sf::String("This is new text"));

			if (button1.widgetEvent == pWidget::WidgetClicked)
				if (window1.visiblity)
					window1.setVisiblity(false);
				else
					window1.setVisiblity(true);
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			if (event.isKeyboardKey(event.keyboard.Escape) ||
				event.isWinClosed()) kernel.window->close();
			if (event.isMouseClickKey(sf::Mouse::Left)) sound.play();
		}
		if (event.isMouseKey(sf::Mouse::Right)) music.setStop();

		progressBar.setValue(slider.getValue()); //[!]

		if (event.type == sf::Event::LostFocus || !kernel.window->hasFocus())
		{ 
			music.setPause();
			sound.stop();
			for (VidIt it = videos.begin(); it != videos.end(); ++it)
				it->second->setPause();
			delay(FOCUS_DELAY);
			continue;
		}
		if (kernel.window->hasFocus())
		{
			for (VidIt it = videos.begin(); it != videos.end(); ++it)
				if (!event.isVideoPlay(*(it->second)))
					it->second->play();
			if (!event.isMusicPlay(music)) music.play();
		}

		startDisplay();

		for (ObjIt it = objects.begin(); it != objects.end(); ++it)
			it->second->display();

		kernel.window->draw(gui); //[!]

		endDisplay();
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (ObjIt it = objects.begin(); it != objects.end(); ++it)
	{
		kernel.print("Deleting objects: " + it->first, INFO);
		delete it->second;
		it->second = NULL;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------