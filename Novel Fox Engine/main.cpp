//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main.cpp										   Главный файл игрового движка
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "Engine/engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Kernel &kernel = ng::Kernel::init(); // Для наглядности получаем свою ссыль
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main()
{
	XMLNode node = kernel.loadFirstScene();
	Scene *scene = new Scene(node);
	
	while (kernel.window->isOpen())
	{
		while (kernel.window->pollEvent(kernel.event))
		{
			kernel.eventUpdate(scene); //Update стандартных событий

			if (kernel.event.isMouseKey(sf::Mouse::Left))
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
		}

		if (kernel.lostFocus())
		{
			if (scene) scene->stopMedia();
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
