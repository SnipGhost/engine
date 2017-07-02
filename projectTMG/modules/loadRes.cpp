#include "loadRes.hpp"

void loadXMLComposer(std::string file) 
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

			//[TO DO] Создавать объекты "Сцена" <-----
			//        |
			//        -- Прогружать по информации данной "сценой"
			//           |
			//           -- Отрисовывать загруженное
			//              |
			//              -- Очищать загруженное

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