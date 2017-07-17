//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main.cpp										   Главный файл игрового движка
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "Engine/engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ng::Kernel &kernel = ng::Kernel::init(); // Для наглядности получаем свою ссылб
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main()
{
	ng::Scene scene;
	scene.loadScene();
	
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

		kernel.band1->display();
		kernel.band2->display();

		kernel.endDisplay();
	}

	scene.clear();

	return EXIT_SUCCESS;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
