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
#define VERSION "0.09"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <type_traits>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sfeMovie/Movie.hpp>
#include "../Modules/tinyxml2.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef OS_IS_WIN // Разные настройки в зависимости от платформы
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
#define PARAMS_COUNT 8           // Количество настроек считываемых из конфига
#define FOCUS_DELAY 500          // Пауза в цикле при потере приложением фокуса
#define NVIDIA_DRIVER 1          // Использовать в компиляции NVIDIA библиотеку
#define MAX_LAYER 10             // Максимальное допустимое значение слоя
#define C_LAYERS (MAX_LAYER*2+1) // Количество всех слоев
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define ABS(x) (((x) < 0) ? (-(x)) : (x)) // Абсолютное значение
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace ng
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	typedef tinyxml2::XMLElement* XMLNode;
	typedef std::unordered_map<std::string, std::string> SaveHash;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	extern const char *DEFAULT[PARAMS_COUNT*2]; // Настройки по-умолчанию
	extern const bool RES_PARAMS[PARAMS_COUNT]; // Добавлять к настрокам пути
	enum TAGS { NONE, CRIT, WARN, NORM, INFO }; // Метки для сообщений лога
	enum { _delay = 0, _layer, _width, _height, _alpha, _time, _size, _x, _y, 
		   _scale, _angle, _volume, _speed, _loop, _smooth, _visible, _layermotion, _id,
		   _src, _text, _style, _color, _fontId, _command };
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class LogStream
	{
		protected:
			std::ostream *log;     // Поток вывода лога
			bool isExtOS;          // Открыт ли поток вовне?
			unsigned int tag_mask; // Маска вывода сообщений
			//-----------------------------------------------------------------
		public:
			LogStream(unsigned int mask = SHOW_ALL_TAG);
			LogStream(std::ostream &os, unsigned int mask = SHOW_ALL_TAG);
			LogStream(std::string file, unsigned int mask = SHOW_ALL_TAG);
			~LogStream();
			//-----------------------------------------------------------------
			bool check();             // Проверка на успешность открытия потока
			void setTagMask(unsigned int mask = SHOW_ALL_TAG);
			//-----------------------------------------------------------------
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
	// Глобальные часы
	class Clock : public sf::Clock
	{
	public:
		int getMilliSecond(); // Выдаём время с момента запуска в миллисекундах
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Класс событий/проверок на события
	class Event : public sf::Event
	{
	public:
		sf::Keyboard keyboard;
		sf::Mouse mouse;
		bool isKeyboardKey(sf::Keyboard::Key keyboard); // Данная клавиша нажата?
		bool isWinClosed(); // Окно закрыто?
		bool isMouseClickKey(sf::Mouse::Button mouse); // Данная кнопка кликнута?
		bool isMouseKey(sf::Mouse::Button mouse); // Данная кнопка нажата?
		bool isMusicPlay(sf::Music *music); // Данная музыка играет?
		bool isVideoPlay(sfe::Movie *video); // Данное видео воспроизводится?
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Преобъявление классов для Kernel
	class Font;
	class Sound;
	class Shape;
	class Scene;
	class Music;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Класс-ядро игрового движка
	class Kernel
	{
		private:       
			std::map<std::string, std::string> conf; // Конфигурация
			Kernel();                                // Конструктор синглтона
			Kernel(const Kernel& root) = delete;
			Kernel & operator=(const Kernel&) = delete;
			//-----------------------------------------------------------------
		public:
			std::string version;				// Версия движка
			LogStream *log;						// Логи программы
			Clock globalClock;					// Глобальное время
			tinyxml2::XMLDocument *doc;			// XML-документ сценария
			sf::RenderWindow *window;			// SFML-окно
			ng::Event event;					// События
			std::map<std::string, Font*> fonts;	// Набор шрифтов
			std::map<std::string, Music*> music;// Набор музыки
			ng::Sound *click;					// Звук для клика
			sf::Vector2f devScreen;				// Размеры среды разработки
			sf::Vector2f screen;				// Размеры монитора
			sf::Vector2f factor;				// Коэффициент между мониторами
			ng::Shape *band1, *band2;			// Полосы сокрытия
			//ng::Shape *transition;				// Чёрное полотно перехода
			XMLNode node;					    // Общая текущая node
			Scene *scene;						// Общая текущая сцена
			SaveHash saveHash;              // Хеш-сохранения
			//-----------------------------------------------------------------
			static Kernel & init();                   // Instance-метод
			~Kernel();                                // Обязательный деструктор
			bool parseConfig(std::string file);       // Загрузить конфигурацию
			std::string operator[] (std::string key); // Выдает конфигурацию
			sf::Vector2f getMouse();				  // Запрос координат мыши
			//-----------------------------------------------------------------
			void loadGameHash();         // Загрузка сохранения в saveHash
			void saveGameHash();		 // Сохранение игры в файл сохранения
			void updateAll();            // Обновить всё, что есть в списке
			void eventUpdate();			 // Отследить обычные события
			void sceneUpdate();			 // Отследить событие переключения
			bool hasFocus();             // Фокус на приложении
			bool lostFocus();            // Фокус на приложении потерян
			void startDisplay();         // Начало отрисовки
			void endDisplay();	         // Конец отрисовки
			void displayUI();            // Отрисовать интерфейс
			void loadSpecData();         // Загрузить шрифты, звук клика и т.д.
			void clear();                // Очистка особых объеков ядра
			XMLNode loadFirstScene();    // Загрузить первую сцену
			//-----------------------------------------------------------------
			XMLNode parseXML(XMLNode node, const char *tag);
			XMLNode getNextXMLNode(XMLNode node, const char *tag); // Выдаём следующую ноду с нужным тегом
			//-----------------------------------------------------------------
			template<typename T> void print(T msg, size_t tag = NONE)
			{
				log->print(msg, tag);
			}
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	extern Kernel &kernel;          // Глобально объявляем наличие объекта ядра
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct FontData
	{
		std::string src;		 // Путь до ресурса
		std::string id;			 // Идентификатор шрифта
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct ResData
	{
		int delay;               // Параметр задержки		0
		int layer;               // Слой отображения		1
		int width;               // Ширина объекта			2
		int height;              // Высота объекта			3
		int alpha;               // Прозрачность			4
		int time;                // Время                   5
		unsigned int size;       // Размер текста			6
		float x;                 // Позиция по X			7
		float y;                 // Позиция по Y			8
		float scale;             // Масштаб (sX = sY)		9
		float angle;             // Угол поворота           10
		float volume;            // Громкость				11
		float speed;             // Быстрота музыки         12
		bool loop;               // Зацикливание			13
		bool smooth;             // Размытие				14
		bool visible;            // Видимость				15
		bool layermotion;		 // Движение слоёв			16
		std::string id;          // Идентификатор объекта	17
		std::string src;         // Путь до ресурса			18
		std::string text;        // Содержание текста		19		
		std::string style;       // Стиль текста			20
		std::string color;       // Цвет текста				21	
		std::string fontId;      // Идентификатор шрифта	22
		std::string command;     // Команда					23

		uint32_t bitMask;        // Битовая маска изменений
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	FontData getFontData(XMLNode tNode);		// Распарсить FONT-ноду
	ResData getData(XMLNode node);				// Распарсить ЛЮБУЮ-ноду
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Класс шрифтов
	class Font : public sf::Font
	{
	public:
		std::string id;					// Id шрифта
		Font(std::string src);			// Загрузить шрифт самостоятельно
		Font(FontData fd);				// Загрузить шрифт из главной FontData
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Класс музыки/действий над музыкой
	class Music: public sf::Music
	{
		public:
			std::string state;			// Текущее состояние музыки
			bool playable;				// Нужно ли проигрывать музыку [!] // Без надобности
			float volume;				// Установленное значение громкости
			float volumeNow;			// Значение громкости сейчас [изменяется]
			bool loop;					// Повторение
			float speed;				// Скорость проигрывания

			bool first;					// Логика для общих действий
			int timeDo;					// Время действия
			int firstTime;
			int nextTime;
			int difference;				// Разница между двума предыдущими параметрами
			int tact;					// Такт изменения

			Music(ResData rd);			// Загрузить музыку из главной ResData
			void edit(ResData rd);		// Изменить текущие настройки
			bool setMusic(std::string src); // Установить музыку
			void update();				// Обновить состояние музыки
			friend std::ostream & operator << (std::ostream &os, const Music *m); // Составление правильного вывода
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Класс звуков/действий над звуками
	class Sound: public sf::Sound
	{
		protected:
			sf::SoundBuffer buffer;
		public:
			float volume;				// Установленное значение громкости
			bool playable;				// Нужно ли проигрывать музыку [!] // Без надобности
			float speed;				// Скорость проигрывания

			Sound(std::string id, std::string src, float volume = 100);
			Sound(ResData rd);			// Загрузить звук из главной ResData
			void edit(ResData rd);		// Изменить текущие настройки
			bool setSound(std::string src, float volume); // Установить звук
			friend std::ostream & operator << (std::ostream &os, const Sound *s); // Составление правильного вывода
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Класс "видимого" объекта и действий над ним
	class Displayable
	{
		protected:
			int layer;
			bool layermotion;
			std::string id;
			sf::Vector2f positionObj;
			float scaleObj;
			std::string color;
			int alpha;
			std::string style;
			int width;
			int height;
			float angle;
		public:
			struct PosScale
			{
				sf::Vector2f pos;
				sf::Vector2f scale;
				PosScale() {}
				PosScale(sf::Vector2f a, sf::Vector2f b) : pos(a), scale(b) {}
				PosScale(float a, float b, float c, float d): pos(a, b), scale(c, d) {}
			} posScale, origin;
			
			virtual ~Displayable();
			sf::Vector2f getPositionObj() { return positionObj; } // Выдать текущую позицию объекта
			float getScaleObj() { return scaleObj; } // Выдать текущий размер объекта	      
			bool visible;				// Текущая видимость объекта
			std::string getId() { return id; } // Выдать id текущего объекта
			virtual bool isMouseAbove() = 0; // Если мышка над данным объектом
			virtual void edit(ResData rd) = 0; // Изменить текущие настройки
			void doLayerMotion(sf::Transformable *obj); // Произвести в каждом объекте setLayerMotion
			virtual void display(sf::RenderWindow *win = kernel.window) = 0; // Кинуть в очередь отображаемых объектов
			virtual void setLayerMotion() {} // Произвести перемещение layer в зависимости от мышки
			virtual std::ostream & print(std::ostream &os) = 0; // Вывод информации в log
			friend std::ostream & operator << (std::ostream &os, Displayable *s); // Составление правильного вывода
			virtual void setResize();	// Установить размеры в зависимости от нового окна
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Класс простых отображаемых фигур
	class Shape: public sf::RectangleShape
	{
	protected:
		bool isColorAdd;
		int alpha;
		sf::Vector2f size;
	public:
		Shape() { isColorAdd = true; }
		Shape(sf::Vector2f winSize);	// Чёрное полотно перехода во весь экран
		Shape(sf::Color clr, int pos, sf::Vector2f winSize, sf::Vector2f devSize); //Ограничивающие полосы
		void setOutlineShape(sf::Vector2f size, sf::Vector2f pos);
		void display(sf::RenderWindow *win = kernel.window); // Кинуть в очередь отображаемых объектов
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Класс текстур(спрайтов) и действий над ними
	class Sprite: public sf::Sprite, public ng::Displayable
	{
		protected:
			ng::Shape outlineShape;
			sf::Color color;
			sf::Texture texture;
			sf::RenderStates renderStates;
		public:
			Sprite() {}
			Sprite(std::string id, std::string src, bool smooth = true); // Загрузить sprite самостоятельно
			Sprite(ResData rd);			// Загрузить sprite из главной ResData
			bool setStrTexture(std::string src, bool smooth); // Установить текстуру
			void setAlpha(int alpha);	// Установить прозрачность данного объекта
			void setRenderStates(std::string style); // Установить параметры отображения
			void display(sf::RenderWindow *win = kernel.window); // Кинуть в очередь отображаемых объектов
			bool isMouseAbove();		// Если мышка над данным объектом
			void edit(ResData rd);		// Изменить текущие настройки
			void setResize();			// Установить размеры в зависимости от нового окна
			void computeLayerScale();	// Просчитать размеры в зависимости от нового окна
			void setLayerMotion();		// Произвести перемещение layer в зависимости от мышки
			std::ostream & print(std::ostream &os); // Вывод информации в log
			friend std::ostream & operator << (std::ostream &os, Sprite &s); // Составление правильного вывода
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Класс анимации(GIF) и действий над ней
	class AnimateSprite: public ng::Sprite
	{
		protected:
			ng::Shape outlineShape;
			int lastTime;				// Предыдущее время смены кадра
			unsigned int xPozAnim;		// Текущее положение анимации по X
			unsigned int yPozAnim;		// Текущее положение анимации по Y
			unsigned int xEnd;			// Номер конечного положения по X
			unsigned int yEnd;			// Номер конечного положения по Y
			unsigned int sideWidth;		// Ширина кадра
			unsigned int sideHeight;	// Высота кадра
			int delay;					// Время между кадрами в миллисекундах
		public:
			AnimateSprite(std::string id, std::string src, bool smooth = true); // Загрузить animatesprite самостоятельно
			AnimateSprite(ResData rd);	// Загрузить animatesprite из главной ResData
			void setAnimation(int frameHeight, int frameWidth = 0, int delay = 40); // Установить анимацию
			void update();				// Обновить состояние анимации
			bool isMouseAbove();		// Если мышка над данным объектом
			void edit(ResData rd);		// Изменить текущие настройки
			void display(sf::RenderWindow *win = kernel.window); // Кинуть в очередь отображаемых объектов
			std::ostream & print(std::ostream &os); // Вывод информации в log
			friend std::ostream & operator << (std::ostream &os, AnimateSprite &s); // Составление правильного вывода
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Класс текста/действий над текстом
	class Text: public sf::Text, public ng::Displayable
	{
		private:
			ng::Shape outlineShape;
			std::string fontId;
			unsigned int size;
		public:
			Text() {}
			Text(ResData rd);			// Загрузить text из главной ResData
			Text(std::string id, int layer, std::string text, std::string fontId,
			     bool layermotion, bool visible, float x, float y, float scale,
				 unsigned int size, std::string color, int alpha, std::string style, float angle); // Загрузить text самостоятельно
			void setText(std::string id, int layer, std::string text, std::string fontId,
				 bool layermotion, bool visible, float x, float y, float scale,
				 unsigned int size, std::string color, int alpha, std::string style, float angle); // Установить текст
			void setColorText(std::string color, int alpha); // Установить цвет текста
			void setStyleText(std::string style); // Установить стиль текста
			bool isMouseAbove();		// Если мышка над данным объектом
			void edit(ResData rd);		// Изменить текущие настройки
			void display(sf::RenderWindow *win = kernel.window); // Кинуть в очередь отображаемых объектов
			void setResize();			// Установить размеры в зависимости от нового окна
			void setLayerMotion();		// Произвести перемещение layer в зависимости от мышки
			std::ostream & print(std::ostream &os); // Вывод информации в log
			friend std::ostream & operator << (std::ostream &os, Text &t); // Составление правильного вывода
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Класс текста/действий над многострочным текстом
	class SmartText: public ng::Displayable
	{
		private:
			std::string text;
			std::string fontId;
			unsigned int size;

			ng::Text *textExample;			// Текст для настроек
			float interval;					// Междустрочный интервал
			std::vector<Text*> textVector;	// Вектор из готовых строк
		public:
			SmartText(ResData rd);			// Загрузить smarttext из главной ResData
			void setSmartText(ResData &rd); // Установить переносящийся текст
			bool isMouseAbove();			// Если мышка над данным объектом
			void edit(ResData rd);			// Изменить текущие настройки
			void display(sf::RenderWindow *win = kernel.window); // Кинуть в очередь отображаемых объектов
			void setResize();				// Установить размеры в зависимости от нового окна
			void setLayerMotion();			// Произвести перемещение layer в зависимости от мышки
			std::ostream & print(std::ostream &os); // Вывод информации в log
			friend std::ostream & operator << (std::ostream &os, SmartText &t); // Составление правильного вывода
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Класс видео/действий над видео
	class Video: public sfe::Movie, public ng::Displayable
	{
		protected:
			ng::Shape outlineShape;
			bool loopVideo;
			sf::RenderStates renderStates;
		public:
			Video(std::string src, int width, int height,
				float x = 0, float y = 0, float volume = 100, bool loop = false); // Загрузить video из главной ResData
			Video(ResData rd);
			bool setVideo(std::string src, int width, int height,
				float x, float y, float volume, bool loop); // Загрузить video самостоятельно
			void setLoop(bool loop);	// Установить зацикливание видео
			void setPause();			// Поставить видео на паузу
			bool isMouseAbove();		// Если мышка над данным объектом
			void edit(ResData rd);		// Изменить текущие настройки
			void setRenderStates(std::string style); // Установить параметры отображения
			void display(sf::RenderWindow *win = kernel.window); // Кинуть в очередь отображаемых объектов
			void setResize();			// Установить размеры в зависимости от нового окна
			void computeLayerScale();	// Просчитать размеры в зависимости от нового окна
			void setLayerMotion();		// Произвести перемещение layer в зависимости от мышки
			std::ostream & print(std::ostream &os); // Вывод информации в log
			friend std::ostream & operator << (std::ostream &os, Video &v); // Составление правильного вывода
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Класс шейдеров(визуальных эффектов над спрайтами)
	class Shader : public sf::Shader
	{
	public:
		Shader();
		//Shader(ResData rd);
		friend std::ostream & operator << (std::ostream &os, const Shader *s); // Составление правильного вывода
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Класс сцены(объект, включающий в себя все остальные классы)
	class Scene
	{
		friend ng::Event;
		friend ng::Kernel;

		protected:
			bool firstEvent;
			bool next;
			std::string idNextScene;		// Выдаем id следующей scene
			std::vector<Displayable*> layers[C_LAYERS]; // Вектор с объектами, кроме видео
			typedef std::map<std::string, ng::Video*>::iterator VidIt; // Map с видео
			typedef std::map<std::string, ng::Music*>::iterator MusIt; // Map с музыкой
			typedef std::map<std::string, ng::Sound*>::iterator SouIt; // Map со звуками

		public:
			XMLNode eventNode;
			std::map<std::string, ng::Displayable*> objects;	// Map с объектами
			std::map<std::string, ng::Video*> videos;			// Map с видео
			std::map<std::string, ng::Sound*> sounds;			// Map со звуками

			Scene() {}
			Scene(XMLNode node);
			~Scene();
			//bool sceneReady;				// Готовность сцены
			int tEvent;						// Время задержки в EVENT-тэге
			int saveTTEvent;				// Сохранение времени для вычислений
			void loadScene(XMLNode scene);  // Загрузка ресурсов сценария
			bool doEvent(XMLNode scene);	// Проход по event для исполнения
			bool isEvent();					// Если нашли Event 
			bool isIfValid();
			void startMedia();				// Запуск остановленных объектов
			void stopMedia();				// Остановка объектов
			void displayAll();              // Отобразить все объекты сцены
			void clear();					// Очистка всех составляющих сцены
	};
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif /* ENGINE_HPP */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~