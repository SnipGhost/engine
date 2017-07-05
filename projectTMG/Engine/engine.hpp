//-----------------------------------------------------------------------------
// engine.hpp                                                   Описание движка
//-----------------------------------------------------------------------------
#pragma once 
#ifndef ENGINE_HPP
#define ENGINE_HPP
//-----------------------------------------------------------------------------
#include "../Modules/macros.h"
//-----------------------------------------------------------------------------
#ifdef OS_IS_WIN
	#include <windows.h>
	#define RES_PATH std::string("Resources/")
	#define APP_ICON "icon.png"
#else
	#ifdef DEBUG
		#define RES_PATH std::string("Resources/")
	#else
		#include "pathfinder.hpp"
		#define RES_PATH findPath()
	#endif
	#define APP_ICON "icon-mac.png"
#endif
//-----------------------------------------------------------------------------
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../Modules/tinyxml2.hpp"
#include <iostream>
#include <fstream>
#include <string>
//-----------------------------------------------------------------------------
#define GETBIT(x,pos) (((x) & ( 0x1 << (pos) )) !=0)
#define SHOW_ALL_TAG 31
//-----------------------------------------------------------------------------
namespace ng
{
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
	//struct AnimateSpriteData
	//{
	//	float x;
	//	float y;
	//	float scale;
	//	int layer;
	//	bool smooth;
	//	std::string id;
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
	tinyxml2::XMLElement* parseXML(std::string file);
	tinyxml2::XMLElement* getSpriteXMLNode(tinyxml2::XMLElement* SPRITE);
	void loadXMLComposer(std::string file);
	SpriteData getSpriteData(tinyxml2::XMLElement *spNode, std::string path);
	MusicData getMusicData(tinyxml2::XMLElement *mNode, std::string path);
	SoundData getSoundData(tinyxml2::XMLElement *sNode, std::string path);
	TextData getTextData(tinyxml2::XMLElement *tNode, std::string path);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Clock: public sf::Clock
	{
	 public:
		int getMilliSecond();
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class LogStream
	{
	 private:
		std::ostream *log;
		bool isExtOS;
		unsigned int tag_mask;
	 public:
		static enum TAGS { NONE, CRIT, WARN, NORM, INFO };
		LogStream(unsigned int mask = SHOW_ALL_TAG);
		LogStream(std::ostream &os, unsigned int mask = SHOW_ALL_TAG);
		LogStream(std::string &file, unsigned int mask = SHOW_ALL_TAG);
		~LogStream();

		bool check();
		void setTagMask(unsigned int mask = SHOW_ALL_TAG);
		void print(std::string msg, size_t tag = 0);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Icon: public sf::Image
	{
	 public:
		Icon(std::string src);
		bool setIcon(std::string src);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Sprite: public sf::Sprite
	{
	 protected:
		sf::Texture texture;
		unsigned int layer;
		std::string id;
	 public:
		Sprite(){}
		Sprite(std::string src, bool smooth = true);
		Sprite(SpriteData sd);
		bool setStrTexture(std::string src, bool smooth);
		void change(SpriteData sd);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class AnimateSprite: public ng::Sprite
	{
	 private:
		int lastTime;   // Предыдущее время смены кадра
		int numFrame;   // Текущий номер кадра
		int sideWidth;  // Ширина кадра
		int sideHeight; // Высота кадра
		int delay;      // Время между кадрами в миллисекундах
	 public:
		AnimateSprite(std::string src, bool smooth = true):Sprite(src, smooth)
		{
			lastTime = 0;
			numFrame = 1;
		}
		/*AnimateSprite(AnimateSpriteData asd);*/
		bool setStrTexture(std::string src, bool smooth);
		void setAnimation(int frameWidth, int frameHeight, int delay);
		void update();
		void draw(sf::RenderWindow window);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Music:public sf::Music
	{
	 public:
		Music(std::string src, float volume = 100, bool loop = true); 
		Music(MusicData md); //TO DO: Показатель volume одинаков для всей музыки
		bool setMusic(std::string src, float volume, bool loop);
		//void change(); //TO DO: Изменение volume
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Sound:public sf::Sound
	{
	 private:
		sf::SoundBuffer buffer;
	 public:
		Sound( std::string src, float volume = 100); 
		Sound(SoundData sod); //TO DO: Показатель volume одинаков для всех звуков
		bool setSound(std::string src, float volume);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Text:public sf::Text
	{
	 private:
		sf::Font font;
		std::map <std::string, int> mapping;
	 public:
	    Text(std::wstring text, float x, float y, int size, std::string path, std::string color = "black");
		Text(TextData td);
	};
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	extern Clock globalClock;
	extern LogStream log;
	extern sf::RenderWindow win;
};
//-----------------------------------------------------------------------------
#endif /* ENGINE_HPP */
//-----------------------------------------------------------------------------