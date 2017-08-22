//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// engine.cpp                         Реализации ядра и вспомогательных функций
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
#include "../Data/loading.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#if defined(OS_IS_WIN) && defined(NVIDIA_DRIVER)
	extern "C" { _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001; }
#endif
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Настройки движка по-умолчанию
const char *ng::DEFAULT[PARAMS_COUNT*2] = \
{
	"scenario",       "scenario/script.xml",   // [0] Путь до скрипта
	"screen_x",       "1280",                  // [1] Ширина окна
	"screen_y",       "720",                   // [2] Высота окна
	"screen_mode",    "7",                     // [3] Режим окна
	"window_name",    "NOVEL FOX ENGINE",      // [4] Название окна
	"app_icon",       DEFAULT_APP_ICON,        // [5] Иконка приложения
	"anti_aliasing",  "8",                     // [6] Уровень сглаживания
	"frame_limit",    "30"                     // [7] Ограничение FPS
};
const bool ng::RES_PARAMS[PARAMS_COUNT] = { 1, 0, 0, 0, 0, 1, 0, 0 };
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Kernel &ng::kernel = Kernel::init();       // Создаем ядро, загружаем настройки
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Kernel::Kernel()
{	
	#if defined(OS_IS_WIN) && ! defined(DEBUG)
		#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
		system("chcp 1251 > nul");
		HWND hWnd = GetConsoleWindow();
		ShowWindow(hWnd, SW_HIDE);
	#endif
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	log = new LogStream(RES_PATH + LOG_FILE);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::string line = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
	log->print(line + "~~~~~~~~~~~~~~~~~~~~[Engine log]", INFO);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	version = VERSION;
	log->print("NOVEL FOX ENGINE v" + version, NORM);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Установка начальных значений из файла конфигурации
	if (!parseConfig(RES_PATH + CONFIG_FILE)) exit(EXIT_FAILURE);
	devScreen.x = std::stof(conf["devScreen_x"].c_str());
	devScreen.y = std::stof(conf["devScreen_y"].c_str());
	int screen_mode = std::atoi(conf["screen_mode"].c_str());
	switch (screen_mode)
	{
	case 8:
	{
		sf::VideoMode videoMode;
		screen.x = (float)videoMode.getDesktopMode().width;
		screen.y = (float)videoMode.getDesktopMode().height;
		break;
	}
	case 7:
	{
		screen.x = std::stof(conf["screen_x"].c_str());
		screen.y = std::stof(conf["screen_y"].c_str());
		break;
	}
	default:
	{
		log->print("INVALID SCREEN MODE!", WARN);
		break;
	}
	}
	int anti_aliasing = std::atoi(conf["anti_aliasing"].c_str());
	int frame_limit = std::atoi(conf["frame_limit"].c_str());
	int vert_sync = std::atoi(conf["vert_sync"].c_str());
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Установка коэффициента размера окна разработки и использования
	factor.x = screen.x / devScreen.x;
	factor.y = screen.y / devScreen.y;
	log->print("Resize: (" + std::to_string(factor.x) + "; " + 
		std::to_string(factor.y) + ")");
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Настройка и создание окна
	sf::ContextSettings setting;
	setting.majorVersion = 2;
	setting.minorVersion = 1;
	setting.antialiasingLevel = anti_aliasing;
	sf::VideoMode videoMode((int)screen.x, (int)screen.y);
	const char *winName = conf["window_name"].c_str();
	window = new sf::RenderWindow(videoMode, winName, screen_mode, setting);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Установка вертикальной синхронизации
	if (vert_sync) window->setVerticalSyncEnabled(true);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Окно закрузки [Исправление прозрачного окна]
	sf::Texture texture;
	texture.loadFromMemory(loadingTexture, sizeof(loadingTexture));
	texture.setSmooth(true);
	sf::Sprite loading(texture);
	float sizeTX = (float)loading.getTextureRect().width;
	float sizeTY = (float)loading.getTextureRect().height;
	float indentX = window->getSize().x / 2 - sizeTX / 2;
	float indentY = window->getSize().y / 2 - sizeTY / 2;
	loading.setPosition(indentX, indentY);
	window->draw(loading);
	window->display();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (window->isOpen()) 
		log->print("Window open", INFO);
	else
	{
		std::string msg = "Failed to open window\n";
		msg += "\tVideo mode:\t" + conf["screen_x"] + "x" + conf["screen_y"];
		msg += "\n\tAnti-Aliasing:\t" + conf["anti_aliasing"];
		msg += "\n\tWindow name:\t" + conf["window_name"];
		log->print(msg, CRIT);
		exit(EXIT_FAILURE);	
	}
	window->setFramerateLimit(frame_limit); // Установка ограничения FPS
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Прикрепление иконки к приложению
	sf::Image icon;
	if(!icon.loadFromFile(conf["app_icon"]))
		log->print("Failed to load icon " + conf["app_icon"], WARN);
	else 
		log->print("Icon loaded " + conf["app_icon"], NORM);
	window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Открытие файла сценария
	doc = new tinyxml2::XMLDocument();
	doc->LoadFile((conf["scenario"]).c_str());
	if (doc->Error())
	{
		log->print("Failed to read script: " + conf["scenario"], CRIT);
		exit(EXIT_FAILURE);	
	}
	else log->print("Script loaded", NORM);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Создание первоначальных полос сокрытия
	sf::Vector2f winsize(window->getSize());
	band1 = new Shape(sf::Color::Black, 1, winsize, devScreen);
	band2 = new Shape(sf::Color::Black, 2, winsize, devScreen);
	log->print("Bands created", NORM);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Создание чёрного полотна перехода
	transition = new Shape(winsize); // [!]
	log->print("Transition screen created", NORM);

	loadSpecData();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Kernel & Kernel::init()
{
	static Kernel k;
	return k;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Kernel::~Kernel()
{
	log->print("Unloading the kernel", INFO);
	if (band1) delete band1;
	if (band2) delete band2;
	log->print("Deleting bands is complete", NORM);
	if (transition) delete transition; // [!]
	log->print("Deleting transition screen is complete", NORM);
	if (doc) delete doc;
	log->print("Closing the script is complete", NORM);
	if (window) delete window;
	log->print("Closing the window is complete", NORM);
	if (log) delete log;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Kernel::parseConfig(std::string file)
{
	std::ifstream fin(file);
	if (fin.fail())
	{
		log->print("Failed to open config-file: " + file, CRIT);
	}
	else log->print("Open config-file: " + file, INFO);
	char line[MAX_LINE];
	char *key, *value;
	while(!fin.fail() && fin.getline(line, MAX_LINE-1))
	{
		if (strlen(line) <= 2) continue;
		size_t i = 0;
		while ((line[i] == ' ' || line[i] == '\t') && (i < strlen(line))) ++i;
		if (line[i] == '#') continue;
		key = strtok(line, CONF_DELIMS);
		if (i != 0) key = line+i;
		i = strlen(key)-1;
		while ((key[i] == ' ' || key[i] == '\t') && (i > 0)) --i;
		if (i != strlen(key)-1) key[i+1] = '\0';
		value = strtok(NULL, CONF_DELIMS);
		if (value[0] == ' ' || value[0] == '\t') value = value+1;
		conf[std::string(key)] = std::string(value);
		log->print(std::string(key) + " = " + conf[key], NORM);
	}
	for (int i = 0; i < PARAMS_COUNT*2; i += 2)
	{
		if (!conf.count(DEFAULT[i]) || (conf[DEFAULT[i]].length() == 0))
		{
			conf[DEFAULT[i]] = DEFAULT[i+1];
			log->print(std::string(DEFAULT[i]) + " taken by default", WARN);
		}
		if (RES_PARAMS[i/2]) conf[DEFAULT[i]] = RES_PATH + conf[DEFAULT[i]];
	}
	fin.close();
	return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
sf::Vector2f Kernel::getMouse()
{
	return window->mapPixelToCoords(sf::Mouse::getPosition(*window));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::string Kernel::operator [] (std::string key)
{
	return conf[key];
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Стандартные события
void Kernel::eventUpdate()
{
		if (event.isKeyboardKey(event.keyboard.Escape) || event.isWinClosed())
			window->close();

		if (event.type == sf::Event::Resized)
		{
			screen.x = (float)window->getSize().x;
			screen.y = (float)window->getSize().y;
			factor.x = screen.x / devScreen.x;
			factor.y = screen.y / devScreen.y;

			log->print("Resize: (" + std::to_string(factor.x) + "; " + 
				std::to_string(factor.y) + ")");

			window->setView(sf::View(sf::FloatRect(0, 0, 
				         (float)event.size.width, (float)event.size.height)));

			for (auto &layer : scene->layers)
			{
				for (auto &obj : layer)
				{
					obj->setResize();
				}
			}

			sf::Vector2f winsize(window->getSize());

			//Пересоздание полос сокрытия
			delete band1;
			delete band2;
			band1 = new Shape(sf::Color::Black, 1, winsize, devScreen);
			band2 = new Shape(sf::Color::Black, 2, winsize, devScreen);

			delete transition; // [!]
			transition = new Shape(winsize); // [!]
		}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Kernel::sceneUpdate()
{
	bool clickMouseLeft = false;
	bool clickEnter = false;
	if (event.isMouseClickKey(sf::Mouse::Left)) clickMouseLeft = true;
	if (event.isKeyboardKey(sf::Keyboard::Return)) clickEnter = true; //Return - Enter
	if ((clickMouseLeft || clickEnter || scene->tEvent == 0)/* && scene->sceneReady*/) // [!]
	{
		for (auto &sound : scene->sounds) // Выключение звука, если насильно переключили
			if (sound.second)
			{
				sound.second->playable = false;
				sound.second->stop();
			}

		if (scene->tEvent != 0 && click) click->play();

		if (clickMouseLeft) print("Mouse click: (" + std::to_string(event.mouseButton.x) + "; "
			+ std::to_string(event.mouseButton.y) + ")", INFO);
		if (clickEnter) print("Keyboard click: Enter", INFO);
		if (scene->tEvent == 0) print("Event timeout ", INFO);

		if (scene->doEvent(node)) {

			delete scene;
			scene = nullptr;

			if (node && node->FirstChildElement("JUMP")) // Если есть node и написали Jump
			{
				std::string id = "next";
				if (node->FirstChildElement("JUMP")->Attribute("id")) // Если в Jump есть атрибут id
				{
					id = node->FirstChildElement("JUMP")->Attribute("id");
					node = getNextXMLNode(node, "SCENE", id);
				}
				else // Если "забыли" написать атрибут id, то на следующую сцену
				{
					print("~~> GO TO NEXT SCENE", NORM);
					node = getNextXMLNode(node, "SCENE");
				}
			}
			else // Если Jump не нашли, то на следующую сцену
			{
				print("~~> GO TO NEXT SCENE", NORM);
				if (node) node = getNextXMLNode(node, "SCENE");
			}

			if (node) // Если есть node, на которую можно перейти
			{
				scene = new Scene(node);
			}
			else // Если нет node, на которую можно перейти, то грузим всё сначала
			{
				node = loadFirstScene(); // В будущем самая первая node будет меню
				if (node) scene = new Scene(node);
			}

		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Kernel::updateAll()
{
	while (window->pollEvent(event))
	{
		eventUpdate();
		sceneUpdate();
	}

	if (scene->tEvent > 0) // Если нашли Event с time
	{
		scene->tEvent -= globalClock.getMilliSecond() - scene->saveTTEvent;
		scene->saveTTEvent = globalClock.getMilliSecond();
		if (scene->tEvent < 0) scene->tEvent = 0;
	}
	else if (scene->tEvent != -1)
	{
		sceneUpdate();
	}

	// Другие update
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Если окно потеряло фокус
bool Kernel::hasFocus()
{
	return window->hasFocus();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Если окно потеряло фокус
bool Kernel::lostFocus()
{
	return (event.type == sf::Event::LostFocus || !window->hasFocus());
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Стандарт: начало отображения
void Kernel::startDisplay()
{
	window->pushGLStates();
	window->clear();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Стандарт: конец отображения
void Kernel::endDisplay()
{
	displayUI();
	window->popGLStates();
	window->display();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Отображение скрывающих полосок
void ng::Kernel::displayUI()
{
	band1->display();
	band2->display();
	//transition->display(); // [!]
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Kernel::loadSpecData()
{
	XMLNode node = parseXML(doc->FirstChildElement("SCRIPT"), "FONT");
	while (node != NULL)
	{
		FontData data = getFontData(node);
		fonts[data.id] = new Font(data);
		log->print("Loaded font: " + data.src, INFO);
		node = getNextXMLNode(node, "FONT");
	}
	node = parseXML(doc->FirstChildElement("SCRIPT"), "CLICK");
	if (node != NULL)
	{
		ResData data = getData(node);
		click = new Sound(data.id, data.src, data.volume);
		log->print("Loaded click sound: " + data.src, INFO);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Kernel::clear()
{
	for (auto &font : fonts) 
		if (font.second) delete font.second;
	log->print("Deleting fonts is complete", NORM);
	if (kernel.click) delete kernel.click;
	log->print("Deleting click sound is complete", NORM);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
XMLNode ng::Kernel::loadFirstScene()
{
	return parseXML(kernel.doc->FirstChildElement("SCRIPT"), "SCENE");
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
XMLNode Kernel::parseXML(XMLNode node, const char *tag)
{	
	return node->FirstChildElement(tag);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
XMLNode Kernel::getNextXMLNode(XMLNode node, const char *tag, std::string id)
{
	XMLNode saveNode = node;
	if (id != "next")
	{
		bool correctTag = false;
		XMLNode nextScene;
		while (!correctTag)
		{
			const char *idNextScene = nullptr;
			nextScene = node->NextSiblingElement(tag);
			if (nextScene)
			{ 
				idNextScene = node->NextSiblingElement(tag)->Attribute("id");
				if (!strcmp(idNextScene, id.c_str()))
				{
					kernel.print("~~> JUMP TO SCENE " + (std::string)idNextScene, NORM);
					return node->NextSiblingElement(tag);
				}
				node = node->NextSiblingElement(tag);
			}
			else
			{
				correctTag = true;
			}
		}
	}
	return saveNode->NextSiblingElement(tag);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::ostream & ng::operator << (std::ostream & os, Displayable * s)
{
	return s->print(os);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~