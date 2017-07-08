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
	#ifdef OS_IS_WIN
		SET_LOCALE;
		#ifndef DEBUG
			HWND hWnd = GetConsoleWindow();
			ShowWindow(hWnd, SW_HIDE);
		#endif
	#endif
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	log = new LogStream("main.log");
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (!parseConfig(CONFIG_FILE)) exit(EXIT_FAILURE);
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
		log->print("Окно запущено", INFO);
	else
	{
		log->print("Ошибка при открытии окна", CRIT);
		std::string vm = conf["screen_x"] + "x" + conf["screen_y"];
		log->print("\t video mode:    " + vm, CRIT);
		log->print("\t anti-Aliasing: " + conf["anti_aliasing"], CRIT);
		log->print("\t window name:   " + conf["window_name"], CRIT);
		exit(EXIT_FAILURE);	
	}
	window->setFramerateLimit(frame_limit);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ng::Icon icon;
	if(!icon.loadFromFile(conf["app_icon"]))
		log->print("Failed load icon " + conf["app_icon"], INFO);
	else 
		log->print("Icon loaded " + conf["app_icon"], INFO);
	window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	log->print("Icon set " + conf["app_icon"], INFO);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	doc = new tinyxml2::XMLDocument();
	doc->LoadFile((conf["scenario"]).c_str());
	if (doc->Error())
	{
		log->print("Ошибка при чтении сценария: " + conf["scenario"], CRIT);
		exit(EXIT_FAILURE);	
	}
	else log->print("Сценарий загружен", INFO);
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
	log->print("Выгрузка ядра", INFO);
	if (doc != NULL) delete doc;
	log->print("Удаление XML документа завершено", INFO);
	if (window != NULL) delete window;
	log->print("Удаление окна завершено", INFO);
	if (log != NULL) delete log;
}
//-----------------------------------------------------------------------------
bool Kernel::parseConfig(std::string file)
{
	std::ifstream fin(file);
	if (fin.fail())
	{
		log->print("Ошибка при загрузке конфигурации: " + file, CRIT);
		return 0;
	}
	else log->print("Файл конфигурации открыт: " + file, INFO);
	char line[MAX_LINE];
	char *key, *value;
	while(fin.getline(line, MAX_LINE-1))
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
		log->print(std::string(key) + " = " + conf[key]); // Проверка
	}
	for (int i = 0; i < PARAMS_COUNT*2; i += 2)
	{
		if (!conf.count(DEFAULT[i]) || (conf[DEFAULT[i]].length() == 0))
		{
			conf[DEFAULT[i]] = DEFAULT[i+1];
			log->print(std::string(DEFAULT[i]) + " взято по-умолчанию", WARN);
		}
		if (RES_PARAMS[i/2]) conf[DEFAULT[i]] = RES_PATH + conf[DEFAULT[i]];
	}
	fin.close();
	return 1;
}
//-----------------------------------------------------------------------------
void Kernel::print(std::string msg, size_t tag)
{
	log->print(msg, tag);
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
SpriteData ng::getSpriteData(XMLNode spNode, std::string path)
{
	const char *x = spNode->Attribute("x");
	const char *y = spNode->Attribute("y");
	const char *src = spNode->Attribute("src");
	const char *scale = spNode->Attribute("scale");
	const char *layer = spNode->Attribute("layer");
	const char *smooth = spNode->Attribute("smooth");
	SpriteData res;
	res.x = std::stof(x);
	res.y = std::stof(y);
	res.src = path + std::string(src);
	res.scale = std::stof(scale);
	res.layer = std::atoi(layer);
	res.smooth = ((strcmp(smooth, "true") == 0) ? true : false);
	return res;
}
//-----------------------------------------------------------------------------