//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main.cpp										   Главный файл игрового движка
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "Engine/engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Kernel &kernel = ng::Kernel::init(); // Для наглядности получаем свою ссылку
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main()
{
	kernel.node = kernel.loadFirstScene();
	kernel.scene = new Scene(kernel.node);
	
	while (kernel.window->isOpen())
	{
		kernel.updateAll();

		if (kernel.lostFocus())
		{
			if (kernel.scene) kernel.scene->stopMedia();
			continue;
		}

		if (kernel.hasFocus())
		{
			if (kernel.scene) kernel.scene->startMedia();
		}

		kernel.startDisplay();
		if (kernel.scene) kernel.scene->displayAll();
		kernel.endDisplay();
	}

	if (kernel.scene) delete kernel.scene;
	kernel.clear();

	return EXIT_SUCCESS;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
