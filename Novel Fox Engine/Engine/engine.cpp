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
SpriteData ng::getSpriteData(XMLNode spNode)
{
	const char *x = spNode->Attribute("x");
	const char *y = spNode->Attribute("y");
	const char *id = spNode->Attribute("id");
	const char *src = spNode->Attribute("src");
	const char *scale = spNode->Attribute("scale");
	const char *layer = spNode->Attribute("layer");
	const char *smooth = spNode->Attribute("smooth");

	SpriteData res;
	res.id = id;
	res.layer = std::atoi(layer);
	res.src = RES_PATH + std::string(src);
	(x) ? res.x = std::stof(x) : res.x = 0;
	(y) ? res.y = std::stof(y) : res.y = 0;
	(scale) ? res.scale = std::stof(scale) : res.scale = 1;
	(smooth) ? res.smooth = CONVTRUE(smooth) : res.smooth = 0;
	return res;
}
//-----------------------------------------------------------------------------
AnimateSpriteData ng::getAnimateSpriteData(XMLNode asdNode)
{
	const char *x = asdNode->Attribute("x");
	const char *y = asdNode->Attribute("y");
	const char *id = asdNode->Attribute("id");
	const char *src = asdNode->Attribute("src");
	const char *scale = asdNode->Attribute("scale");
	const char *layer = asdNode->Attribute("layer");
	const char *smooth = asdNode->Attribute("smooth");
	const char *height = asdNode->Attribute("height");
	const char *width = asdNode->Attribute("width");
	const char *delay = asdNode->Attribute("delay");

	AnimateSpriteData res;
	res.id = id;
	res.layer = std::atoi(layer);
	res.src = RES_PATH + std::string(src);
	res.frameHeight = std::atoi(height);
	res.frameWidth = std::atoi(width);
	(x) ? res.x = std::stof(x) : res.x = 0;
	(y) ? res.y = std::stof(y) : res.y = 0;
	(scale) ? res.scale = std::stof(scale) : res.scale = 1;
	(smooth) ? res.smooth = CONVTRUE(smooth) : res.smooth = false;
	(delay) ? res.ms = std::atoi(delay) : res.ms = 40;
	return res;
}
//-----------------------------------------------------------------------------
MusicData ng::getMusicData(XMLNode mNode)
{
	const char *volume = mNode->Attribute("volume");
	const char *loop = mNode->Attribute("loop");
	const char *src = mNode->Attribute("src");
	const char *cmd = mNode->Attribute("cmd");

	MusicData res;
	res.src = RES_PATH + std::string(src);
	(volume) ? res.volume = std::stof(volume) : res.volume = 100;
	(loop) ? res.loop = CONVTRUE(loop) : res.loop = true;
	(cmd) ? res.cmd = cmd : res.cmd = "play";
	return res;
}
//-----------------------------------------------------------------------------
SoundData ng::getSoundData(XMLNode sNode)
{
	const char *volume = sNode->Attribute("volume");
	const char *src = sNode->Attribute("src");

	SoundData res;
	res.src = RES_PATH + std::string(src);
	(volume) ? res.volume = std::stof(volume) : res.volume = 100;
	return res;
}
//-----------------------------------------------------------------------------
FontData ng::getFontData(XMLNode tNode)
{
	const char *src = tNode->Attribute("src");
	const char *id = tNode->Attribute("id");

	FontData res;
	res.id = id;
	res.src = RES_PATH + src;
	return res;
}
//-----------------------------------------------------------------------------
TextData ng::getTextData(XMLNode tNode, std::map<std::string, Font*> fonts)
{
	const char *text = tNode->Attribute("text");
	const char *color = tNode->Attribute("color");
	const char *layer = tNode->Attribute("layer");
	const char *namePerson = tNode->Attribute("name");
	const char *size = tNode->Attribute("size");
	const char *font = tNode->Attribute("font");
	const char *x = tNode->Attribute("x");
	const char *y = tNode->Attribute("y");
	const char *id = tNode->Attribute("id");

	TextData res;
	res.id = id;
	res.text = text;
	res.layer = std::atoi(layer);
	(namePerson) ? res.namePerson = namePerson : res.namePerson = "";
	(color) ? res.color = color : res.color = "black";
	(x) ? res.x = std::stof(x) : res.x = 0;
	(y) ? res.y = std::stof(y) : res.y = 0;
	(size) ? res.size = std::atoi(size) : res.size = 1;
	(font) ? res.font = fonts[font] : res.font = fonts["standart"];
	return res;
}
//-----------------------------------------------------------------------------
VideoData ng::getVideoData(XMLNode vNode)
{
	const char *x = vNode->Attribute("x");
	const char *y = vNode->Attribute("y");
	const char *id = vNode->Attribute("id");
	const char *src = vNode->Attribute("src");
	const char *layer = vNode->Attribute("layer");
	const char *volume = vNode->Attribute("volume");
	const char *loop = vNode->Attribute("loop");
	const char *width = vNode->Attribute("width");
	const char *height = vNode->Attribute("height");

	VideoData res;
	res.id = id;
	res.layer = std::atoi(layer);
	res.width = std::stof(width);
	res.height = std::stof(height);
	res.src = RES_PATH + std::string(src);
	(x) ? res.x = std::stof(x) : res.x = 0;
	(y) ? res.y = std::stof(y) : res.y = 0;
	(volume) ? res.volume = std::stof(volume) : res.volume = 100;
	(loop) ? res.loop = CONVTRUE(loop) : res.loop = false;
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