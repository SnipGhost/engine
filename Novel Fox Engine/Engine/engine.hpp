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
#define VERSION "0.06.5"
//-----------------------------------------------------------------------------
#ifdef OS_IS_WIN
	#include <windows.h>
	#include <time.h>
	#define delay(ms) Sleep(ms)
	#define RES_PATH std::string("Resources/")
	#define DEFAULT_APP_ICON "icon.png"
	#define CONFIG_FILE std::string("config.ini")  // Путь до конфигурации
	#define SET_LOCALE system("chcp 1251 > nul")
	#define LOG_FILE std::string("main.log")
#else
	#include <unistd.h>
	#define delay(ms) usleep((ms)*1000)
	#ifdef DEBUG
		#define RES_PATH std::string("Resources/")
		#define CONFIG_FILE std::string("config.ini")
		#define LOG_FILE std::string("main.log")
	#else
		#include "pathfinder.hpp"
		#define RES_PATH findPath()
		#define CONFIG_FILE (findPath()+"config.ini")
		#define LOG_FILE (findPath()+"main.log")
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
#include <sfeMovie/Movie.hpp>
#include "../Modules/tinyxml2.hpp"
//-----------------------------------------------------------------------------
#define GETBIT(x,pos) (((x) & ( 1 << (pos) )) !=0) // Получить pos бит числа x
//-----------------------------------------------------------------------------
#define MAX_LINE 256     // Максимальный размер строки
#define CONF_DELIMS "="  // Разделители в конфиге
#define SHOW_ALL_TAG 31  // 0b11111
#define PARAMS_COUNT 9   // Количество настроек
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
		LogStream(std::string file, unsigned int mask = SHOW_ALL_TAG);
		~LogStream();

		bool check();
		void setTagMask(unsigned int mask = SHOW_ALL_TAG);
		template <typename T> void print(T msg, size_t tag = NONE)
		{
			const size_t TAG_COUNT = 5;
			const char *TAGM[TAG_COUNT] = {
				"[ ] ", // [0 NONE]
				"[!] ", // [1 CRIT]
				"[-] ", // [2 WARN]
				"[+] ", // [3 NORM]
				"[i] "  // [4 INFO]
			};
			check();
			if (tag >= TAG_COUNT)
			{
				print("Unknown tag", 2);
				print(msg, 0);
				return;
			}
			if (GETBIT(tag_mask, TAG_COUNT - tag - 1) == 0)
				return;
			*log << TAGM[tag] << msg << std::endl;
		}
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
		bool isVideoPlay(sfe::Movie &video);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Kernel
	{
	 private:       
		std::map<std::string, std::string> conf;     // Конфигурация
		Kernel();                                    // Конструктор синглтона
		Kernel(const Kernel& root) = delete;
		Kernel & operator=(const Kernel&) = delete;
	 public:
		LogStream *log;                              // Логи программы
		Clock globalClock;                           // Счетчик времени
		tinyxml2::XMLDocument *doc;                  // XML-документ сценария
		sf::RenderWindow *window;                    // SFML-окно
		std::string version;

		static Kernel & init();                      // Instance-метод
		~Kernel();
		bool parseConfig(std::string file);          // Загрузить конфигурацию
		template <typename T> void print(T msg, size_t tag = NONE)
		{
			log->print(msg, tag);
		}
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
		std::string id;
		std::string src;
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct AnimateSpriteData
	{
		float x;
		float y;
		int frameHeight;
		int frameWidth;
		int ms;
		int layer;
		float scale;
		bool smooth;
		std::string id;
		std::string src;
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct MusicData
	{
		float volume;
		bool loop;
		std::string src;
		std::string cmd;
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct SoundData
	{
		float volume;
		std::string src;
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct TextData
	{
		std::string text;
		std::string color;
		std::string namePerson;
		unsigned int size;
		int layer;
		float x;
		float y;
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct VideoData
	{
		float x;
		float y;
		bool loop;
		int layer;
		float width;
		float volume;
		float height;
		std::string id;
		std::string src;
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	XMLNode parseXML(const char *tag);
	XMLNode getNextXMLNode(XMLNode node, const char *tag);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	SpriteData getSpriteData(XMLNode spNode);
	AnimateSpriteData getAnimateSpriteData(XMLNode asNode);
	MusicData getMusicData(XMLNode mNode);
	SoundData getSoundData(XMLNode sNode);
	TextData getTextData(XMLNode tNode);
	VideoData getVideoData(XMLNode vNode);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Icon: public sf::Image
	{
	 public:
		Icon(){}
		Icon(std::string src);
		bool setIcon(std::string src);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Music: public sf::Music
	{
	private:
		float volume;
	public:
		Music(std::string src, float volume = 100, bool loop = true);
		Music(MusicData md); //TODO: Показатель volume одинаков для всей музыки
		bool setMusic(std::string src, float volume, bool loop);
		void setPause();
		void setStop();
		//void change(); //TODO: Изменение volume
		friend std::ostream & operator << (std::ostream &os, const Music &m);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Sound: public sf::Sound
	{
	protected:
		sf::SoundBuffer buffer;
	public:
		Sound(std::string src, float volume = 100);
		Sound(SoundData sod); //TODO: Показатель volume одинаков для всех звуков
		bool setSound(std::string src, float volume);
		friend std::ostream & operator << (std::ostream &os, const Sound &s);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	extern void startDisplay();
	extern void endDisplay();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Displayable // Абстрактный класс
	{
		protected:
			int layer;
		public:
			unsigned int getLayer() { return layer; }
			virtual void display(sf::RenderWindow *win = kernel.window) = 0;
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Sprite: public sf::Sprite, public ng::Displayable
	{
	 protected:
		int layer;
		sf::Texture texture;
		std::string id;
	 public:
		Sprite() {}
		Sprite(std::string src, bool smooth = true);
		Sprite(SpriteData sd);
		bool setStrTexture(std::string src, bool smooth);
		void change(SpriteData sd);
		void display(sf::RenderWindow *win = kernel.window);
		friend std::ostream & operator << (std::ostream &os, const Sprite &s);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class AnimateSprite: public ng::Sprite
	{
	 protected:
		 int layer;
		int lastTime;            // Предыдущее время смены кадра
		unsigned int numFrame;   // Текущий номер кадра
		unsigned int sideWidth;  // Ширина кадра
		unsigned int sideHeight; // Высота кадра
		int delay;               // Время между кадрами в миллисекундах
	 public:
		AnimateSprite(std::string src, bool smooth = true);
		AnimateSprite(AnimateSpriteData asd);
		void setAnimation(int frameHeight, int frameWidth = 0, int delay = 40);
		void update();
		void display(sf::RenderWindow *win = kernel.window);
		friend std::ostream & operator << (std::ostream &os, const AnimateSprite &s);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Text: public sf::Text, public ng::Displayable
	{
	 protected:
		 int layer;
		sf::Font font;
		std::string id;
		std::map <std::string, int> mapping;
	 public:
		Text(std::string text, std::string color, float x, float y, unsigned int size);
		Text(TextData td);
		bool setText(std::string text, std::string color, float x, float y, unsigned int size);
		void display(sf::RenderWindow *win = kernel.window);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Video: public sfe::Movie, public ng::Displayable
	{
	 private:
		 int layer;
		 bool loopVideo;
		 std::string id;
	 public:
		Video(std::string src, float width, float height,
			float x = 0, float y = 0, float volume = 100, bool loop = false);
		Video(VideoData vd);
		bool setVideo(std::string src, float width, float height,
			float x, float y, float volume, bool loop);
		void setLoop(bool loop);
		void setPause();
		void display(sf::RenderWindow *win = kernel.window);
		friend std::ostream & operator << (std::ostream &os, const Video &v);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};
//-----------------------------------------------------------------------------
#endif /* ENGINE_HPP */
//-----------------------------------------------------------------------------