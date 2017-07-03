//-----------------------------------------------------------------------------
// engine.cpp                                                 Реализации Engine
//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
tinyxml2::XMLDocument *doc;
//-----------------------------------------------------------------------------
tinyxml2::XMLElement* ng::parseXML(std::string file)
{
	doc = new tinyxml2::XMLDocument();
	doc->LoadFile((file).c_str());
	return doc->FirstChildElement("SCRIPTGAME")->FirstChildElement("SPRITE");
}
//-----------------------------------------------------------------------------
tinyxml2::XMLElement* ng::getSpriteXMLNode(tinyxml2::XMLElement* SPRITE)
{
	return SPRITE->NextSiblingElement("SPRITE");
}
//-----------------------------------------------------------------------------
void ng::loadXMLComposer(std::string file) 
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile((file).c_str());
	tinyxml2::XMLElement* CHAPTER = doc.FirstChildElement("SCRIPTGAME")->FirstChildElement("CHAPTER");
	while (CHAPTER != NULL)
	{
		std::cout << CHAPTER->Attribute("name") << " | Number: " << CHAPTER->Attribute("number") << std::endl;
		tinyxml2::XMLElement* SCENE = CHAPTER->FirstChildElement("SCENE"); 
		while(SCENE != NULL) 
		{
			const char *number = SCENE->Attribute("number");
			const char *texture = SCENE->Attribute("texture");
			const char *text = SCENE->Attribute("text");
			const char *sound = SCENE->Attribute("sound");
			const char *music = SCENE->Attribute("music");

			std::cout << "SCENE: " << number << std::endl
					  << "Texture: " << texture << std::endl
					  << "Text: " << text << std::endl
					  << "Sound: " << sound << std::endl
					  << "Music: " << music << std::endl << std::endl;

			SCENE = SCENE->NextSiblingElement("SCENE");
		}
		CHAPTER = CHAPTER->NextSiblingElement("CHAPTER");
	}
}
//-----------------------------------------------------------------------------
SpriteData ng::getSpriteData(tinyxml2::XMLElement *spNode, std::string path)
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