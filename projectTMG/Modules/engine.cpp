//-----------------------------------------------------------------------------
// engine.cpp												  Реализации Engine
//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
namespace ng
{

	//sf::Clock clock;
	//sf::Clock clockNG()
	//{
	//	return clock;
	//}

//-----------------------------------------------------------------------------
	std::ostream *log = new std::ofstream("Log/log.log");  
	void showMsg(std::string msg, std::ostream &out)
	{
		out << "[INFO " /*<< time*/ << " ]:  " << msg << std::endl;
	}
//-----------------------------------------------------------------------------
	SpriteData getSpriteData(tinyxml2::XMLElement *spNode, std::string path)
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
	Sprite::Sprite(std::string id, std::string src, bool smooth)
	{
		this->id = id;
		if (!setStrTexture(src, smooth))
			std::cout << "Error" << std::endl;
	}
//-----------------------------------------------------------------------------
	Sprite::Sprite(SpriteData sd)
	{
		this->id = sd.id;
		if (!setStrTexture(sd.src, sd.smooth))
			std::cout << "Error" << std::endl;
		setPosition(sd.x, sd.y);
		setScale(sd.scale, sd.scale);
	}
//-----------------------------------------------------------------------------
	bool Sprite::setStrTexture(std::string src, bool smooth)
	{
		if (!texture.loadFromFile(src))
			return 0;
		texture.setSmooth(smooth);
		setTexture(texture);
		return 1;
	}
}
//-----------------------------------------------------------------------------
//void Sprite::change(tinyxml2::XMLElement) 
//{
//
//}
//-----------------------------------------------------------------------------