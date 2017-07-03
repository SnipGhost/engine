//-----------------------------------------------------------------------------
// engine.hpp                                                   Описание движка
//-----------------------------------------------------------------------------
#pragma once 
#ifndef ENGINE_HPP
#define ENGINE_HPP
//-----------------------------------------------------------------------------
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "tinyxml2.hpp"
#include <iostream>
#include <fstream> 
//-----------------------------------------------------------------------------
namespace ng
{
	/*sf::Clock clockNG();*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//class Kernel
	//{
	//	private:

	//	public:

	//};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	extern std::ostream *log; 
	void showMsg(std::string msg, std::ostream &out = *log);
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
	SpriteData getSpriteData(tinyxml2::XMLElement *spNode, std::string path);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class Sprite: public sf::Sprite
	{
		private:
			sf::Texture texture;
			std::string id;
			unsigned int layer;
		public:
			Sprite(std::string id, std::string src, bool smooth = true);
			Sprite(ng::SpriteData sd);
			bool setStrTexture(std::string src, bool smooth = true);
			//void change(SpriteData sd);
	};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//class Music:public sf::Music
	//{
	//	private:

	//	public:

	//};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//class Sound:public sf::Sound
	//{
	//	private:

	//	public:

	//};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//class Text:public sf::Text
	//{
	//	private:

	//	public:

	//};
};
//-----------------------------------------------------------------------------
#endif /* ENGINE_HPP */
//-----------------------------------------------------------------------------