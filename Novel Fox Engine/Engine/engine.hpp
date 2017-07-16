//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// engine.hpp                                                   Описание движка
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma once 
#ifndef ENGINE_HPP
#define ENGINE_HPP
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "../Modules/macros.h"
#define _CRT_SECURE_NO_WARNINGS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define VERSION "0.08"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <type_traits>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sfeMovie/Movie.hpp>
#include "../Modules/tinyxml2.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef OS_IS_WIN
	#include <windows.h>
	#define delay(ms) Sleep(ms)
	#define RES_PATH std::string("Resources/")
	#define DEFAULT_APP_ICON "icon.png"
#else
	#include <unistd.h>
	#define delay(ms) usleep((ms)*1000)
	#define DEFAULT_APP_ICON "icon-mac.png"
	#ifdef DEBUG
		#define RES_PATH std::string("Resources/")
	#else
		#include "pathfinder.hpp"
		#define RES_PATH findPath()
	#endif
#endif
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define GETBIT(x,pos) (((x) & ( 1 << (pos) )) !=0) // Получить pos бит числа x
#define CONVTRUE(str) (!(strcmp((str), "true")) ? 1 : 0) // Перевод str в bool
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define CONFIG_FILE "config.ini" // Файл конфигурации
#define LOG_FILE "main.log"      // Файл логов
#define MAX_LINE 256             // Максимальный размер строки конфига/файла
#define CONF_DELIMS "="          // Разделители ключ/значение в конфиге
#define SHOW_ALL_TAG 31          // 0b11111 - маска отображения всех сообщений
#define PARAMS_COUNT 9           // Количество настроек считываемых из конфига
#define FOCUS_DELAY 500          // Пауза в цикле при потере приложением фокуса
#define NVIDIA_DRIVER 1          // Использовать в компиляции NVIDIA библиотеку
#define WS_X ((float)kernel.window->getSize().x)
#define WS_Y ((float)kernel.window->getSize().y)
#define KWS_X ((float)kernel.window->getSize().x / kernel.devScreen.x)
#define KWS_Y ((float)kernel.window->getSize().y / kernel.devScreen.y)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef tinyxml2::XMLElement* XMLNode;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
		template<typename T> void print(T msg, size_t tag = NONE)
		{
			static const size_t TAG_COUNT = 5;
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
	struct FontData
	{
		std::string src;
		std::string id;
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Font : public sf::Font
	{
		public:
			std::string id;
			Font(std::string src);
			Font(FontData fd);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Kernel
	{
		private:       
			std::map<std::string, std::string> conf; // Конфигурация
			Kernel();                                // Конструктор синглтона
			Kernel(const Kernel& root) = delete;
			Kernel & operator=(const Kernel&) = delete;
		public:
			LogStream *log;                          // Логи программы
			Clock globalClock;                       // Счетчик времени
			tinyxml2::XMLDocument *doc;              // XML-документ сценария
			sf::RenderWindow *window;                // SFML-окно
			std::string version;
			ng::Event event;
			std::map<std::string, Font*> fonts;

			static Kernel & init();                  // Instance-метод
			~Kernel();
			bool parseConfig(std::string file);      // Загрузить конфигурацию
			sf::Vector2f mouse();					 // Координаты мыши
			sf::Vector2f devScreen;				     // Размеры среды разработки
			template<typename T> void print(T msg, size_t tag = NONE)
			{ log->print(msg, tag); }
			std::string operator[] (std::string key); // Выдает конфигурацию
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	extern Kernel &kernel;          // Глобально объявляем наличие объекта ядра
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct ResData
	{
		int ms;
		int layer;
		int width;
		int height;
		int alpha;
		unsigned int size;
		float x;
		float y;
		float scale;
		float volume;
		bool loop;
		bool smooth;
		std::string id;
		std::string cmd;
		std::string src;
		std::string text;
		std::string color;
		std::string fontId;
		std::string namePerson;
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	XMLNode parseXML(const char *tag);
	XMLNode getNextXMLNode(XMLNode node, const char *tag);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	FontData getFontData(XMLNode tNode);
	ResData getResData(XMLNode node);
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
			Music(const Music &copy) {}; // TODO: констурктор копий
			Music(std::string src, float volume = 100, bool loop = true);
			Music(ResData rd); //TODO: Показатель volume одинаков для всей музыки
			bool setMusic(std::string src, float volume, bool loop);
			void setPause();
			void setStop();
			//void change(); //TODO: Изменение volume
			friend std::ostream & operator << (std::ostream &os, const Music *m);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Sound: public sf::Sound
	{
		protected:
			sf::SoundBuffer buffer;
		public:
			Sound(std::string src, float volume = 100);
			Sound(ResData rd); //TODO: Показатель volume одинаков для всех звуков
			bool setSound(std::string src, float volume);
			friend std::ostream & operator << (std::ostream &os, const Sound *s);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	extern bool hasFocus();		//Фокус на приложении
	extern bool lostFocus();	//Фокус на приложении потерян
	extern void startDisplay(); //Начало Display
	extern void endDisplay();	//Конец Display
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Displayable
	{
		protected:
			int layer;
			std::string id;
		public:
			struct PosScale
			{
				sf::Vector2f scale;
				sf::Vector2f pos;
			} posScale;
			virtual ~Displayable() 
			{ 
				kernel.print("Deleted displayble object: " + id, INFO); 
			}
			unsigned int getLayer();
			void doLayerMotion(sf::Transformable *obj);
			virtual void display(sf::RenderWindow *win = kernel.window) = 0;
			virtual void setResize() {}
			virtual void setLayerMotion() {}
			virtual std::ostream & print(std::ostream &os) = 0;
			friend std::ostream & operator << (std::ostream &os, Displayable *s);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Displayable::PosScale setResize(sf::Transformable *obj);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Shape : public sf::RectangleShape
	{
	protected:
		sf::Vector2f size;
	public:
		Shape(sf::Color color, std::string pos);	 //Конструктор огран. полос
		void display(sf::RenderWindow *win = kernel.window);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Sprite: public sf::Sprite, public ng::Displayable
	{
		protected:
			sf::Texture texture;
		public:
			Sprite() {}
			Sprite(std::string id, std::string src, bool smooth = true);
			Sprite(ResData rd);
			bool setStrTexture(std::string src, bool smooth);
			/*void change(ResData rd);*/
			void display(sf::RenderWindow *win = kernel.window);
			void setResize();
			void computeLayerScale();
			void setLayerMotion();
			std::ostream & print(std::ostream &os);
			friend std::ostream & operator << (std::ostream &os, Sprite &s);
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
			AnimateSprite(std::string id, std::string src, bool smooth = true);
			AnimateSprite(ResData rd);
			void setAnimation(int frameHeight, int frameWidth = 0, int delay = 40);
			void update();
			/*void change(ResData rd);*/
			void display(sf::RenderWindow *win = kernel.window);
			std::ostream & print(std::ostream &os);
			friend std::ostream & operator << (std::ostream &os, AnimateSprite &s);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Text: public sf::Text, public ng::Displayable
	{
		protected:
			std::map <std::string, int> mapping;
		public:
			Text(ResData rd);
			bool setText(ResData &rd);
			/*void change(ResData rd);*/
			void display(sf::RenderWindow *win = kernel.window);
			void setResize();
			void setLayerMotion();
			//void computeLayerScale();
			std::ostream & print(std::ostream &os);
			friend std::ostream & operator << (std::ostream &os, Text &t);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Video: public sfe::Movie, public ng::Displayable
	{
		protected:
			bool loopVideo;
		public:
			Video(std::string src, int width, int height,
				float x = 0, float y = 0, float volume = 100, bool loop = false);
			Video(ResData rd);
			bool setVideo(std::string src, int width, int height,
				float x, float y, float volume, bool loop);
			void setLoop(bool loop);
			void setPause();
			/*void change(ResData rd);*/
			void display(sf::RenderWindow *win = kernel.window);
			void setResize();
			void setLayerMotion();
			std::ostream & print(std::ostream &os);
			friend std::ostream & operator << (std::ostream &os, Video &v);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/*class Scene
	{
		protected:

		public:

	};*/
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif /* ENGINE_HPP */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~