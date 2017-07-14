//-----------------------------------------------------------------------------
// engine.cpp                         Реализации ядра и вспомогательных функций
//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
Kernel &ng::kernel = Kernel::init();       // Создаем ядро, загружаем настройки
//-----------------------------------------------------------------------------
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
	int screen_x = atoi(conf["screen_x"].c_str());
	int screen_y = atoi(conf["screen_y"].c_str());
	int screen_mode = atoi(conf["screen_mode"].c_str());
	int anti_aliasing = atoi(conf["anti_aliasing"].c_str());
	int frame_limit = atoi(conf["frame_limit"].c_str());
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	sf::ContextSettings setting;
	setting.antialiasingLevel = anti_aliasing;
	sf::VideoMode videoMode(screen_x, screen_y);
	const char *winName = conf["window_name"].c_str();
	window = new sf::RenderWindow(videoMode, winName, screen_mode, setting);
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
}
//-----------------------------------------------------------------------------
Kernel & Kernel::init()
{
	static Kernel k;
	return k;
}
//-----------------------------------------------------------------------------
Kernel::~Kernel()
{
	log->print("Unloading the kernel", INFO);
	if (doc != NULL) delete doc;
	log->print("Closing the script is complete", NORM);
	if (window != NULL) delete window;
	log->print("Closing the window is complete", NORM);
	if (log != NULL) delete log;
}
//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
std::string Kernel::operator [] (std::string key)
{
	return conf[key];
}
//-----------------------------------------------------------------------------
XMLNode ng::parseXML(const char *tag)
{	
	std::string b = kernel["xml_body"];
	return kernel.doc->FirstChildElement(b.c_str())->FirstChildElement(tag);
}
//-----------------------------------------------------------------------------
XMLNode ng::getNextXMLNode(XMLNode node, const char *tag)
{
	return node->NextSiblingElement(tag);
}
//-----------------------------------------------------------------------------
ResData ng::getResData(XMLNode node)
{
	const char *x = node->Attribute("x");
	const char *y = node->Attribute("y");
	const char *id = node->Attribute("id");
	const char *src = node->Attribute("src");
	const char *cmd = node->Attribute("cmd");
	const char *size = node->Attribute("size");
	const char *fontId = node->Attribute("font");
	const char *loop = node->Attribute("loop");
	const char *color = node->Attribute("color");
	const char *scale = node->Attribute("scale");
	const char *layer = node->Attribute("layer");
	const char *width = node->Attribute("width");
	const char *delay = node->Attribute("delay");
	const char *smooth = node->Attribute("smooth");
	const char *height = node->Attribute("height");
	const char *volume = node->Attribute("volume");
	const char *namePerson = node->Attribute("name");
	const char *text = NULL;
	if (!strcmp(node->Name(), "TEXT")) 
		text = node->GetText();

	ResData res;

	(fontId) ? res.fontId = fontId : res.fontId = "standart";
	(delay) ? res.ms = std::atoi(delay) : res.ms = 40;
	(x) ? res.x = std::stof(x) : res.x = 0;
	(y) ? res.y = std::stof(y) : res.y = 0;
	(layer) ? res.layer = std::atoi(layer) : res.layer = 0;
	(loop) ? res.loop = CONVTRUE(loop) : res.loop = false;
	(smooth) ? res.smooth = CONVTRUE(smooth) : res.smooth = 0;
	(scale) ? res.scale = std::stof(scale) : res.scale = 1;
	(width) ? res.width = std::atoi(width) : res.width = 256;
	(height) ? res.height = std::atoi(height) : res.height = 256;
	(volume) ? res.volume = std::stof(volume) : res.volume = 100;
	(size) ? res.size = std::atoi(size) : res.size = 1;
	(id) ? res.id = id : res.id = "NULL";
	(cmd) ? res.cmd = cmd : res.cmd = "play";
	(src) ? res.src = RES_PATH + std::string(src) : res.src = "NULL";
	(text) ? res.text = text : res.text = "NO TEXT";
	(color) ? res.color = color : res.color = "black";
	(namePerson) ? res.namePerson = namePerson : res.namePerson = "";

	return res;
}
//-----------------------------------------------------------------------------
FontData ng::getFontData(XMLNode node)
{
	const char *src = node->Attribute("src");
	const char *id = node->Attribute("id");

	FontData res;
	res.id = id;
	res.src = RES_PATH + src;
	return res;
}
//-----------------------------------------------------------------------------
void ng::startDisplay()
{
	kernel.window->pushGLStates();
	kernel.window->clear();
}
//-----------------------------------------------------------------------------
void ng::endDisplay()
{
	kernel.window->popGLStates();
	kernel.window->display();
}
//-----------------------------------------------------------------------------
void ng::setResize(sf::Transformable *obj)
{
    float x = 1280;
    float y = 720;
    float k = x / y;

    if (WS_X*(1/k) <= WS_Y)
    {
        obj->setPosition(obj->getPosition().x*KWS_X, obj->getPosition().y*KWS_X + (WS_Y - WS_X * (1.0 / k)) / 2);
        obj->setScale(obj->getScale().x*KWS_X, obj->getScale().y*KWS_X);
    }
    else
    {
        obj->setPosition(obj->getPosition().x*KWS_Y + ((WS_X - WS_Y * k) / 2), obj->getPosition().y*KWS_Y);
        obj->setScale(obj->getScale().x*KWS_Y, obj->getScale().y*KWS_Y);
    }
}
//-----------------------------------------------------------------------------