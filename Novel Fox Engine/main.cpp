//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main.cpp										   Главный файл игрового движка
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "Engine/engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ng::Kernel &kernel = ng::Kernel::init(); // Для наглядности получаем свою ссыль
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main()
{
	ng::Scene scene;
	scene.loadScene();
	
	while (kernel.window->isOpen())
	{
		if (kernel.checkEvents()) // Пример события: прошел дождь
		{
			if (scene.objects.count("rain") > 0)
				scene.objects["rain"]->visible = false;

			// На самом деле, музыка почему-то не стопится
			// Поэтому я её тупо удалил ... TODO: FIX! [?]

			if (scene.music.count("music3") > 0)
			{
				delete scene.music["music3"];
				scene.music.erase("music3");
			}

			if (scene.music.count("music2") > 0)
			{
				delete scene.music["music2"];
				scene.music.erase("music2");
			}
		}

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
