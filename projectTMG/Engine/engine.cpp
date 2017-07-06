//-----------------------------------------------------------------------------
// engine.cpp                         Реализации ядра и вспомогательных функций
//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Kernel &ng::kernel = Kernel::init();
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
	int screen_m = atoi(conf["screen_m"].c_str());
	int anti_alias = atoi(conf["anti_alias"].c_str());
	int fps_lim = atoi(conf["fps_lim"].c_str());
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	sf::ContextSettings setting;
	setting.antialiasingLevel = anti_alias;
	sf::VideoMode videoMode(screen_x, screen_y);
	const char *winName = conf["window_n"].c_str();
	window = new sf::RenderWindow(videoMode, winName, screen_m, setting);
	if (window->isOpen()) 
		log->print("Окно запущено", INFO);
	else
	{
		log->print("Ошибка при открытии окна", CRIT);
		std::string vm = conf["screen_x"] + "x" + conf["screen_y"];
		log->print("\t video mode:    " + vm, CRIT);
		log->print("\t anti-Aliasing: " + conf["anti_alias"], CRIT);
		log->print("\t window name:   " + conf["window_n"], CRIT);
		exit(EXIT_FAILURE);	
	}
	window->setFramerateLimit(30);
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
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
		log->print("Ошибка при загрузке конфгурации: " + file, CRIT);
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
		log->print(std::string(key) + " = " + conf[key]);
	}
	// Путь до скрипта со сценарием
	if (!conf.count("scenario")) {
		conf["scenario"] = DEFAULT_SCENARIO;
		log->print("scenario взято по-умолчанию", WARN);
	}
	conf["scenario"] = RES_PATH + conf["scenario"];
	// XML-BODY
	if (!conf.count("xml_body")) {
		conf["xml_body"] = DEFAULT_XML_BODY;
		log->print("xml_body взято по-умолчанию", WARN);
	}
	// Ширина окна
	if (!conf.count("screen_x")) {
		conf["screen_x"] = DEFAULT_SCREEN_X;
		log->print("screen_x взято по-умолчанию", WARN);
	}
	// Высота окна
	if (!conf.count("screen_y")) {
		conf["screen_y"] = DEFAULT_SCREEN_Y;
		log->print("screen_y взято по-умолчанию", WARN);
	}
	// Режим открытия окна
	if (!conf.count("screen_m")) {
		conf["screen_m"] = DEFAULT_SCREEN_M;
		log->print("screen_m взято по-умолчанию", WARN);
	}
	// Название окна
	if (!conf.count("window_n")) {
		conf["window_n"] = DEFAULT_WINDOW_N;
		log->print("window_n взято по-умолчанию", WARN);
	}
	// Уровень сглаживания
	if (!conf.count("anti_alias")) {
		conf["anti_alias"] = DEFAULT_ALIASLVL;
		log->print("anti_alias взято по-умолчанию", WARN);
	}
	// Путь до иконки
	if (!conf.count("app_icon")) {
		conf["app_icon"] = DEFAULT_APP_ICON;
		log->print("app_icon взято по-умолчанию", WARN);
	}
	conf["app_icon"] = RES_PATH + conf["app_icon"];
	// Ограничение FPS
	if (!conf.count("fps_lim")) {
		conf["fps_lim"] = DEFAULT_FPSLIMIT;
		log->print("fps_lim взято по-умолчанию", WARN);
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
	const char *id = spNode->Attribute("id");
	const char *src = spNode->Attribute("src");
	const char *scale = spNode->Attribute("scale");
	const char *layer = spNode->Attribute("layer");
	const char *smooth = spNode->Attribute("smooth");
	SpriteData res;
	res.x = std::stof(x);
	res.y = std::stof(y);
	res.id = std::string(id);
	res.src = path + std::string(src);
	res.scale = std::stof(scale);
	res.layer = std::atoi(layer);
	res.smooth = ((strcmp(smooth, "true") == 0) ? true : false);
	return res;
}
//-----------------------------------------------------------------------------