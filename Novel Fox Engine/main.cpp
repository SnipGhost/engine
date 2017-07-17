//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main.cpp										   Главный файл игрового движка
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "Engine/engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main()
{
	Scene scene;
	scene.loadScene();

	ng::Shape *band1, *band2;
	band1 = new Shape(sf::Color::Black, "top-left");
	band2 = new Shape(sf::Color::Black, "bottom-right");
	kernel.print("Shapes created", NORM);
	
	while (kernel.window->isOpen())
	{
		kernel.checkEvents();

		if (kernel.lostFocus())
		{
			scene.stopMedia();
			delay(FOCUS_DELAY);
			continue;
		}

		if (kernel.hasFocus())
		{
			scene.startMedia();
		}

		kernel.startDisplay();

		scene.displayAll();

		band1->display();
		band2->display();

		kernel.endDisplay();
	}

	scene.clear();

	delete band1, band2;

	return EXIT_SUCCESS;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
