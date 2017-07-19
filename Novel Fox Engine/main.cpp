//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main.cpp										   Главный файл игрового движка
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "Engine/engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Kernel &kernel = ng::Kernel::init(); // Для наглядности получаем свою ссыль
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void example(ng::Scene *scene)
{
	if (scene->objects.count("rain"))
		scene->objects["rain"]->visible = false;

	if (scene->music.count("music3"))
	{
		scene->music["music3"]->stop();
		scene->music["music3"]->playable = false;
	}

	if (scene->music.count("music2"))
	{
		scene->music["music2"]->stop();
		scene->music["music2"]->playable = false;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main()
{
	XMLNode node = kernel.loadFirstScene();
	Scene *scene = new Scene(node);
	
	while (kernel.window->isOpen())
	{
		if (kernel.checkEvents(scene)) // TODO: Возвращать не bool, а enum
		{
			delete scene;
			scene = nullptr;
			if (node) node = kernel.getNextXMLNode(node, "SCENE");
			if (node) scene = new Scene(node);
			else
			{
				node = kernel.loadFirstScene();
				if (node) scene = new Scene(node);
			}
		}

		if (kernel.lostFocus())
		{
			if (scene) scene->stopMedia();
			delay(FOCUS_DELAY);
			continue;
		}

		if (kernel.hasFocus())
		{
			if (scene) scene->startMedia();
		}

		kernel.startDisplay();

		if (scene) scene->displayAll();

		kernel.endDisplay();
	}

	if (scene) delete scene;

	kernel.clear();

	return EXIT_SUCCESS;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
