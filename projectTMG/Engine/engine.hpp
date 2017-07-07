//-----------------------------------------------------------------------------
// engine.hpp                                                   Описание движка
//-----------------------------------------------------------------------------
#pragma once 
#ifndef ENGINE_HPP
#define ENGINE_HPP
//-----------------------------------------------------------------------------
#include "../Modules/macros.h"
#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
#ifdef OS_IS_WIN
	#include <windows.h>
	#define RES_PATH std::string("Resources/")     // Путь до Ресурсов
	#define DEFAULT_APP_ICON "icon.png"
	#define CONFIG_FILE std::string("config.ini")  // Путь до конфигурации
	#define SET_LOCALE system("chcp 1251 > nul")
#else
	#ifdef DEBUG
		#define RES_PATH std::string("Resources/")
		#define CONFIG_FILE std::string("config.ini")  // Путь до конфигурации
	#else
		#include "pathfinder.hpp"
		#define RES_PATH findPath()
		#define CONFIG_FILE findPath() + "config.ini"  // Путь до конфигурации
	#endif
	#define DEFAULT_APP_ICON "icon-mac.png"
#endif
//-----------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../Modules/tinyxml2.hpp"
//-----------------------------------------------------------------------------
#define GETBIT(x,pos) (((x) & ( 1 << (pos) )) !=0) // Получить pos бит числа x
//-----------------------------------------------------------------------------
#define MAX_LINE 256                           // Максимальный размер строки
#define CONF_DELIMS "="                        // Разделители в конфиге
#define SHOW_ALL_TAG 31                        // 0b11111
#define PARAMS_COUNT 9                         // Количество настроек
//-----------------------------------------------------------------------------
typedef tinyxml2::XMLElement* XMLNode;
//-----------------------------------------------------------------------------
namespace ng
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	extern const char *DEFAULT[PARAMS_COUNT*2]; // Настройки по-умолчанию
	extern const bool RES_PARAMS[PARAMS_COUNT]; // Добавлять к настрокам пути
	enum TAGS { NONE, CRIT, WARN, NORM, INFO }; // Метки для сообщений лога
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class LogStream
	{
	 protected:
		std::ostream *log;
		bool isExtOS;
		unsigned int tag_mask;
	 public:
		LogStream(unsigned int mask = SHOW_ALL_TAG);
		LogStream(std::ostream &os, unsigned int mask = SHOW_ALL_TAG);
		LogStream(const char *file, unsigned int mask = SHOW_ALL_TAG);
		~LogStream();

		bool check();
		void setTagMask(unsigned int mask = SHOW_ALL_TAG);
		void print(std::string msg, size_t tag = NONE);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Clock: public sf::Clock
	{
	 public:
		int getMilliSecond();
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Event: public sf::Event
	{
	 public:
		sf::Keyboard keyboard;
		sf::Mouse mouse;
		bool isKeyboardKey(sf::Keyboard::Key keyboard);
		bool isWinClosed();
		bool isMouseClickKey(sf::Mouse::Button mouse);
		bool isMouseKey(sf::Mouse::Button mouse);
		bool isMusicPlay(sf::Music &music);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Kernel
	{
	 private:       
		std::map<std::string, std::string> conf;     // Конфигурация
		Kernel();                                    // Конструктор синглтона
		Kernel(const Kernel& root);
		Kernel & operator=(const Kernel&);
	 public:
		LogStream *log;                              // Логи программы
		Clock globalClock;                           // Счетчик времени
		tinyxml2::XMLDocument *doc;                  // XML-документ сценария
		sf::RenderWindow *window;                    // SFML-окно

		static Kernel & init();                      // Instance-метод
		~Kernel();
		bool parseConfig(std::string file);          // Загрузить конфигурацию
		void print(std::string msg, size_t tag = 0); // Перенаправление на лог
		std::string operator[] (std::string key);    // Выдает конфигурацию
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	extern Kernel &kernel;          // Глобально объявляем наличие объекта ядра
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct SpriteData
	{
		float x;
		float y;
		float scale;
		int layer;
		bool smooth;
		std::string src;
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//struct AnimateSpriteData
	//{
	//	float x;
	//	float y;
	//	float scale;
	//	int layer;
	//	bool smooth;
	//	std::string src;
	//};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct MusicData
	{
		float volume;
		bool loop;
		std::string id;
		std::string src;
		std::string cmd;
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct SoundData
	{
		float volume;
		std::string id;
		std::string src;
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct TextData
	{
		std::string text;
		std::string color;
		std::string namePerson;
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	XMLNode parseXML(const char *tag);
	XMLNode getNextXMLNode(XMLNode node, const char *tag);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	SpriteData getSpriteData(XMLNode spNode, std::string path);
	//MusicData getMusicData(XMLNode mNode, std::string path);
	//SoundData getSoundData(XMLNode sNode, std::string path);
	//TextData getTextData(XMLNode tNode, std::string path);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Icon: public sf::Image
	{
	 public:
		Icon(){}
		Icon(std::string src);
		bool setIcon(std::string src);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Sprite: public sf::Sprite
	{
	 protected:
		sf::Texture texture;
		unsigned int layer;
	 public:
		Sprite() {}
		Sprite(std::string src, bool smooth = true);
		Sprite(SpriteData sd);
		bool setStrTexture(std::string src, bool smooth);
		void change(SpriteData sd);
		void display(sf::RenderWindow *win = kernel.window);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class AnimateSprite: public ng::Sprite
	{
	 protected:
		int lastTime;            // Предыдущее время смены кадра
		unsigned int numFrame;   // Текущий номер кадра
		unsigned int sideWidth;  // Ширина кадра
		unsigned int sideHeight; // Высота кадра
		int delay;               // Время между кадрами в миллисекундах
	 public:
		AnimateSprite(std::string src, bool smooth = true);
		/*AnimateSprite(AnimateSpriteData asd);*/
		bool setStrTexture(std::string src, bool smooth);
		void setAnimation(int frameHeight, int frameWidth = 0, int delay = 40);
		void update();
		void display(sf::RenderWindow *win = kernel.window);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Music:public sf::Music
	{
	 private:
		 float volume;
	 public:
		Music(std::string src, float volume = 100, bool loop = true); 
		Music(MusicData md); //TODO: Показатель volume одинаков для всей музыки
		bool setMusic(std::string src, float volume, bool loop);
		void stopMusic();
		//void change(); //TODO: Изменение volume
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Sound:public sf::Sound
	{
	 protected:
		sf::SoundBuffer buffer;
	 public:
		Sound( std::string src, float volume = 100); 
		Sound(SoundData sod); //TODO: Показатель volume одинаков для всех звуков
		bool setSound(std::string src, float volume);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Text:public sf::Text
	{
	 protected:
		sf::Font font;
		std::map <std::string, int> mapping;
	 public:
		Text(std::wstring text, float x, float y, int size, 
			 std::string path, std::string color = "black");
		Text(TextData td);
		void display(sf::RenderWindow *win = kernel.window);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};
//-----------------------------------------------------------------------------
#endif /* ENGINE_HPP */
//-----------------------------------------------------------------------------