//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// smarttext.cpp     Реализация класса умного текста с автоматическим переносом
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SmartText::SmartText(ResData rd)
{
	id = rd.id;
	layer = rd.layer;
	layermotion = rd.layermotion;
	visible = rd.visible;
	positionObj = sf::Vector2f(rd.x, rd.y);
	scaleObj = rd.scale;
	fontId = rd.fontId;
	size = rd.size;
	color = rd.color;
	alpha = rd.alpha;
	style = rd.style;
	width = rd.width;
	height = rd.height;

	text = new Text(id, layer, rd.text, fontId, layermotion, visible,
		positionObj.x, positionObj.y, scaleObj, size, color, alpha, style);

	interval = text->getLocalBounds().height + height;

	setSmartText(rd);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SmartText::setSmartText(ResData &rd)
{
	std::vector<sf::String> vecWords;
	std::vector<sf::String> vecStrings;

	vecWords = scanWords(rd.text); //Вектор слов
	vecStrings = scanString(width, vecWords, text); //Вектор строк

	for (int i = 0; i < (int)vecStrings.size(); i++)
	{
		text = new Text("line" + i, layer, vecStrings[i], fontId, layermotion, 
			visible, positionObj.x, positionObj.y + interval * i, scaleObj, size, 
			color, alpha, style);
		textVector.push_back(text);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Возвращает вектор слов
std::vector<sf::String> SmartText::scanWords(sf::String str)
{
	std::vector<sf::String> vecStr;
	sf::String buff;

	for (int i = 0; i < (int)str.getSize(); i++)
	{
		if (str[i] != ' ')
		{
			buff += str[i];
		}
		else
		{
			if (buff != "")
			{
				vecStr.push_back(buff);
				buff = "";
			}
		}
	}
	vecStr.push_back(buff);
	return vecStr;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Возвращает вектор готовых строк
std::vector<sf::String> SmartText::scanString(float w, std::vector<sf::String> vecWords_, sf::Text* text)
{
	std::vector<sf::String> vecStr;
	sf::String buff;
	sf::String buff2;

	for (int i = 0; i < (int)vecWords_.size(); i++)
	{
		buff = buff2;
		(buff2 == "") ? buff2 += vecWords_[i] : buff2 += " " + vecWords_[i];
		text->setString(buff2);
		if (text->getLocalBounds().width > w)
		{
			vecStr.push_back(buff);
			buff = "";
			buff2 = vecWords_[i];
		}
	}
	vecStr.push_back(buff2);
	return vecStr;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SmartText::edit(ResData rd) //Доработать
{
	//if (GETBIT(rd.bitMask, _x) || GETBIT(rd.bitMask, _y))
	//{
	//	if (GETBIT(rd.bitMask, _x))
	//	{
	//		positionObj.x = rd.x;
	//		setPosition(rd.x, positionObj.y);
	//		origin = PosScale(rd.x, positionObj.y, scaleObj, scaleObj);
	//	}
	//	if (GETBIT(rd.bitMask, _y))
	//	{
	//		positionObj.y = rd.y;
	//		setPosition(positionObj.x, rd.y);
	//		origin = PosScale(positionObj.x, rd.y, scaleObj, scaleObj);
	//	}
	//	setResize();
	//}
	//if (GETBIT(rd.bitMask, _scale))
	//{
	//	scaleObj = rd.scale;
	//	setScale(rd.scale, rd.scale);
	//	origin = PosScale(positionObj.x, positionObj.y, rd.scale, rd.scale);
	//	setResize();
	//}
	if (GETBIT(rd.bitMask, _size)) 
		for (auto &text : textVector) 
			text->setCharacterSize(rd.size);
	if (GETBIT(rd.bitMask, _fontId)) 
		for (auto &text : textVector) 
			text->setFont(*kernel.fonts[rd.fontId]);
	if (GETBIT(rd.bitMask, _text))
	{
		textVector.clear();
		setSmartText(rd);
	}
	if (GETBIT(rd.bitMask, _color)) 
		for (auto &text : textVector) 
			text->setColorText(rd.color, rd.alpha);
	if (GETBIT(rd.bitMask, _style)) 
		for (auto &text : textVector) 
			text->setStyleText(rd.style);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SmartText::display(sf::RenderWindow *win)
{
	for (auto &text : textVector)
	{
		text->display(win);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::ostream &ng::operator << (std::ostream& os, SmartText &t)
{
	return t.print(os);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::ostream & SmartText::print(std::ostream &os)
{
	//Большинство информации спрашиваем у первой строчки
	sf::Vector2f pos = textVector[0]->getPosition();
	sf::Vector2f scl = textVector[0]->getScale();
	os << id << " [ng::SmartText]" << std::endl;
	os << "\tColor:   \t" << textVector[0]->getFillColor().toInteger() << std::endl;
	os << "\tLayer:   \t" << layer << std::endl;
	os << "\tPosition:\t(" << pos.x << "; " << pos.y << ")" << std::endl;
	os << "\tScale:   \t(" << scl.x << "; " << scl.y << ")" << std::endl;
	return os;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SmartText::setResize()
{
	for (auto &text : textVector)
	{
		text->setResize();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SmartText::setLayerMotion()
{
	for (auto &text : textVector)
	{
		text->setLayerMotion();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~