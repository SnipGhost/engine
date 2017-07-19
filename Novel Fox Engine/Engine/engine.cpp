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
	"xml_body",       "SCRIPTGAME",            // [1] Заглавный тег сценария
	"screen_x",       "1280",                  // [2] Ширина окна
	"screen_y",       "720",                   // [3] Высота окна
	"screen_mode",    "7",                     // [4] Режим окна
	"window_name",    "NOVEL FOX ENGINE",      // [5] Название окна
	"app_icon",       DEFAULT_APP_ICON,        // [6] Иконка приложения
	"anti_aliasing",  "8",                     // [7] Уровень сглаживания
	"frame_limit",    "30"                     // [8] Ограничение FPS
};
const bool ng::RES_PARAMS[PARAMS_COUNT] = { 1, 0, 0, 0, 0, 0, 1, 0, 0 };
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
	version = VERSION;
	log->print("Novel fox engine v" + version, NORM);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (!parseConfig(RES_PATH + CONFIG_FILE)) exit(EXIT_FAILURE);
	int screen_x = std::atoi(conf["screen_x"].c_str());
	int screen_y = std::atoi(conf["screen_y"].c_str());
	devScreen.x = std::stof(conf["devScreen_x"].c_str());
	devScreen.y = std::stof(conf["devScreen_y"].c_str());
	int screen_mode = std::atoi(conf["screen_mode"].c_str());
	int anti_aliasing = std::atoi(conf["anti_aliasing"].c_str());
	int frame_limit = std::atoi(conf["frame_limit"].c_str());
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	sf::ContextSettings setting;
	setting.majorVersion = 2;
	setting.minorVersion = 1;
	setting.antialiasingLevel = anti_aliasing;
	sf::VideoMode videoMode(screen_x, screen_y);
	const char *winName = conf["window_name"].c_str();
	window = new sf::RenderWindow(videoMode, winName, screen_mode, setting);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//Окно закрузки [Исправление прозрачного окна]
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
		msg += "\tvideo mode:\t" + conf["screen_x"] + "x" + conf["screen_y"];
		msg += "\n\tanti-Aliasing:\t" + conf["anti_aliasing"];
		msg += "\n\twindow name:\t" + conf["window_name"];
		log->print(msg, CRIT);
		exit(EXIT_FAILURE);	
	}
	window->setFramerateLimit(frame_limit);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ng::Icon icon;
	if(!icon.loadFromFile(conf["app_icon"]))
		log->print("Failed to load icon " + conf["app_icon"], WARN);
	else 
		log->print("Icon loaded " + conf["app_icon"], NORM);
	window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	doc = new tinyxml2::XMLDocument();
	doc->LoadFile((conf["scenario"]).c_str());
	if (doc->Error())
	{
		log->print("Failed to read script: " + conf["scenario"], CRIT);
		exit(EXIT_FAILURE);	
	}
	else log->print("Script loaded", NORM);
	sf::Vector2f winsize(window->getSize());
	band1 = new Shape(sf::Color::Black, "top-left", winsize, devScreen);
	band2 = new Shape(sf::Color::Black, "bottom-right", winsize, devScreen);
	log->print("Bands created", NORM);
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
bool Kernel::checkEvents()
{
	bool flag = false;
	while (window->pollEvent(event))
	{
		if (event.isKeyboardKey(event.keyboard.Escape) || event.isWinClosed())
			window->close();

		if (event.isMouseClickKey(sf::Mouse::Left))
		{
			if (click) click->play();
			flag = true;
		}
	}
	return flag;
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
void ng::Kernel::displayUI()
{
	band1->display();
	band2->display();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Kernel::loadSpecData()
{
	XMLNode node = parseXML("FONT");
	while (node != NULL)
	{
		FontData data = getFontData(node);
		fonts[data.id] = new Font(data);
		log->print("Loaded font: " + data.src, INFO);
		node = getNextXMLNode(node, "FONT");
	}
	node = parseXML("CLICK");
	if (node != NULL)
	{
		ResData data = getResData(node);
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
XMLNode Kernel::parseXML(const char *tag)
{	
	std::string b = conf["xml_body"];
	return doc->FirstChildElement(b.c_str())->FirstChildElement(tag);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
XMLNode Kernel::getNextXMLNode(XMLNode node, const char *tag)
{
	return node->NextSiblingElement(tag);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Получение и возврат информации по ресурсам
ResData ng::getResData(XMLNode node)
{
	const char *x = node->Attribute("x");
	const char *y = node->Attribute("y");
	const char *id = node->Attribute("id");
	const char *src = node->Attribute("src");
	const char *size = node->Attribute("size");
	const char *loop = node->Attribute("loop");
	const char *fontId = node->Attribute("font");
	const char *alpha = node->Attribute("alpha");
	const char *color = node->Attribute("color");
	const char *scale = node->Attribute("scale");
	const char *style = node->Attribute("style");
	const char *layer = node->Attribute("layer");
	const char *width = node->Attribute("width");
	const char *delay = node->Attribute("delay");
	const char *smooth = node->Attribute("smooth");
	const char *height = node->Attribute("height");
	const char *volume = node->Attribute("volume");
	const char *visible = node->Attribute("visible");
	const char *namePerson = node->Attribute("name");
	const char *text = NULL;
	if (!strcmp(node->Name(), "TEXT")) 
		text = node->GetText();

	ResData res;

	(id) ? res.id = id : res.id = "NULL";
	(x) ? res.x = std::stof(x) : res.x = 0;
	(y) ? res.y = std::stof(y) : res.y = 0;
	(text) ? res.text = text : res.text = "NO TEXT";
	(style) ? res.style = style : res.style = "NULL";
	(color) ? res.color = color : res.color = "black";
	(delay) ? res.ms = std::atoi(delay) : res.ms = 40;
	(size) ? res.size = std::atoi(size) : res.size = 1;
	(loop) ? res.loop = CONVTRUE(loop) : res.loop = false;
	(scale) ? res.scale = std::stof(scale) : res.scale = 1;
	(layer) ? res.layer = std::atoi(layer) : res.layer = 0;
	(fontId) ? res.fontId = fontId : res.fontId = "standart";
	(width) ? res.width = std::atoi(width) : res.width = 256;
	(smooth) ? res.smooth = CONVTRUE(smooth) : res.smooth = 0;
	(height) ? res.height = std::atoi(height) : res.height = 256;
	(volume) ? res.volume = std::stof(volume) : res.volume = 100;
	(visible) ? res.visible = CONVTRUE(visible) : res.visible = true;
	(src) ? res.src = RES_PATH + std::string(src) : res.src = "NULL";
	(namePerson) ? res.namePerson = namePerson : res.namePerson = "";
	(alpha) ? res.alpha = 255 * std::atoi(alpha) / 100 : res.alpha = 255;

	return res;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Получение и возврат информации по шрифтам
FontData ng::getFontData(XMLNode node)
{
	const char *src = node->Attribute("src");
	const char *id = node->Attribute("id");

	FontData res;
	res.id = id;
	res.src = RES_PATH + src;
	return res;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::ostream & ng::operator << (std::ostream & os, Displayable * s)
{
	return s->print(os);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Установка новых значений размера
Displayable::PosScale ng::setResize(sf::Transformable *obj) // TODO: FIX! [?]
{
	Displayable::PosScale ps;

	float k = (float)16 / 9;

	if (WS_X * (1 / k) <= WS_Y)
	{
		float posX = obj->getPosition().x * KWS_X;
		float pozY = obj->getPosition().y * KWS_X + (WS_Y - WS_X * (1 / k)) / 2;
		obj->setPosition(posX, pozY);

		float scaleX = obj->getScale().x * KWS_X;
		float scaleY = obj->getScale().y * KWS_X;
		obj->setScale(scaleX, scaleY);
	}
	else
	{
		float posX = obj->getPosition().x * KWS_Y + ((WS_X - WS_Y * k) / 2);
		float pozY = obj->getPosition().y * KWS_Y;
		obj->setPosition(posX, pozY);

		float scaleX = obj->getScale().x * KWS_Y;
		float scaleY = obj->getScale().y * KWS_Y;
		obj->setScale(scaleX, scaleY);
	}

	ps.pos.x = obj->getPosition().x;
	ps.pos.y = obj->getPosition().y;

	ps.scale.x = obj->getScale().x;
	ps.scale.y = obj->getScale().y;
	return ps;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~