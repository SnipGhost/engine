//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main.cpp										   Главный файл игрового движка
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "Engine/engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ng::Kernel &kernel = ng::Kernel::init(); // Для наглядности получаем свою ссыль
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void example(ng::Scene &scene)
{
	if (scene.objects.count("rain"))
		scene.objects["rain"]->visible = false;

	if (scene.music.count("music3"))
	{
		scene.music["music3"]->stop();
		scene.music["music3"]->playable = false;
	}

	if (scene.music.count("music2"))
	{
		scene.music["music2"]->stop();
		scene.music["music2"]->playable = false;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main()
{
	ng::Scene scene;
	scene.loadScene();
	
	while (kernel.window->isOpen())
	{
		if (kernel.checkEvents()) // TODO: Возвращать не bool, а enum
		{
			example(scene);       // Пример события: прошел дождь
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

		kernel.endDisplay();
	}

	scene.clear();

	kernel.clear();

	return EXIT_SUCCESS;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
