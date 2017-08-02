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
	XMLNode node = kernel.loadFirstScene();
	Scene *scene = new Scene(node);
	
	while (kernel.window->isOpen())
	{
		while (kernel.window->pollEvent(kernel.event))
		{
			kernel.eventUpdate(scene);

			if (kernel.event.isMouseClickKey(sf::Mouse::Left))
			{
				if (kernel.click) kernel.click->play();

				kernel.print("Mouse click: (" + std::to_string(kernel.event.mouseButton.x) + "; "
					+ std::to_string(kernel.event.mouseButton.y) + ")",INFO);

				if (scene->doEvent(node)) {

					delete scene;
					scene = nullptr;

					ResData data;
					if (node && scene->jump(node))
					{
						data = getData(node->FirstChildElement("JUMP"));
						node = kernel.getNextXMLNode(node, "SCENE", data.id);
					}
					else
					{
						kernel.print("~~> GO TO NEXT SCENE", 3);
						if (node) node = kernel.getNextXMLNode(node, "SCENE");
					}

					if (node)
						scene = new Scene(node);
					else
					{
						node = kernel.loadFirstScene();
						if (node) scene = new Scene(node);
					}

				}
			}
		}

		//sf::VideoMode vd;
		//std::cout << vd.getDesktopMode().bitsPerPixel << std::endl; // Выдает текущую битность используемого монитора
		//std::cout << vd.isValid() << std::endl; // Проверяющая функция "Стоит ли Fullscreen?"
		//std::cout << vd.getDesktopMode().isValid() << std::endl; // Проверяет сам монитор на пригодность к Fullscreen? Надо разобраться!
		//std::cout << "Height monitor: " << vd.getDesktopMode().height << " Width monitir: " << vd.getDesktopMode().width << std::endl; // Размер монитора на любой платформе

		//sf::Mouse mouseGlobal;
		//std::cout << mouseGlobal.getPosition().x << " " << mouseGlobal.getPosition().y << std::endl; // Координаты на рабочем столе
		//std::cout << mouseGlobal.getPosition(*kernel.window).x << " " << mouseGlobal.getPosition(*kernel.window).y << std::endl; // Координаты в окне

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
