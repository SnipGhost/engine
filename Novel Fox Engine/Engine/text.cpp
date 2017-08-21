//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// text.cpp                                            Реализация класса текста
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define BOLD sf::Text::Bold
#define STRIKETHROUGH sf::Text::StrikeThrough
#define ITALIC sf::Text::Italic
#define UNDERLINED sf::Text::Underlined
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Text::Text(ResData rd)
{
	setText(rd.id, rd.layer, rd.text, rd.fontId, rd.layermotion, rd.visible, 
		    rd.x, rd.y, rd.scale, rd.size, rd.color, rd.alpha, rd.style);

	origin = PosScale(rd.x, rd.y, rd.scale, rd.scale);
	setResize();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Text::Text(std::string id, int layer, std::string text, std::string fontId,
bool layermotion, bool visible, float x, float y, float scale, 
unsigned int size, std::string color, int alpha, std::string style)
{
	setText(id, layer, text, fontId, layermotion, visible, x, y, scale, size, color, alpha, style);

	origin = PosScale(x, y, scale, scale);
	setResize();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Text::setText(std::string _id, int _layer, std::string text, std::string fontId,
bool _layermotion, bool _visible, float x, float y, float scale, 
unsigned int size, std::string color, int alpha, std::string style)
{
	setString(sf::String::fromUtf8(text.begin(), text.end()));
	setFont(*kernel.fonts[fontId]);

	id = _id;
	layer = _layer;
	layermotion = _layermotion;
	visible = _visible;
	positionObj = sf::Vector2f(x, y);
	scaleObj = scale;

	//setOutlineThickness(2);				//Обводка
	//setOutlineColor(sf::Color::Blue);     //Цвет обводки

	setColorText(color, alpha);
	setCharacterSize(size);
	if (style != "NULL") setStyleText(style);

	return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Text::setColorText(std::string color, int alpha)
{
	if (color == "red")
		setFillColor(sf::Color(255, 0, 0, alpha));
	if (color == "green")
		setFillColor(sf::Color(0, 128, 0, alpha));
	if (color == "blue")
		setFillColor(sf::Color(0, 0, 255, alpha));
	if (color == "yellow")
		setFillColor(sf::Color(255, 255, 0, alpha));
	if (color == "white")
		setFillColor(sf::Color(255, 255, 255, alpha));
	if (color == "black")
		setFillColor(sf::Color(0, 0, 0, alpha));

	//ДОБАВИТЬ ФУНКЦИЮ RGB!
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Text::setStyleText(std::string style)
{
	char *Ptr = strtok((char*)style.c_str(), " ");

	unsigned int styleNum = 0;

	while (Ptr)
	{
		if (!strcmp(Ptr, "bold"))
			styleNum += BOLD;
		if (!strcmp(Ptr, "strikethrough"))
			styleNum += STRIKETHROUGH;
		if (!strcmp(Ptr, "italic"))
			styleNum += ITALIC;
		if (!strcmp(Ptr, "underlined"))
			styleNum += UNDERLINED;
		Ptr = strtok(0, " ");
	}

	setStyle(styleNum);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Text::edit(ResData rd)
{
	if (GETBIT(rd.bitMask, _x) || GETBIT(rd.bitMask, _y))
	{
		if (GETBIT(rd.bitMask, _x))
		{
			positionObj.x = rd.x;
			setPosition(rd.x, positionObj.y);
			origin = PosScale(rd.x, positionObj.y, scaleObj, scaleObj);
		}
		if (GETBIT(rd.bitMask, _y))
		{
			positionObj.y = rd.y;
			setPosition(positionObj.x, rd.y);
			origin = PosScale(positionObj.x, rd.y, scaleObj, scaleObj);
		}
		setResize();
	}
	if (GETBIT(rd.bitMask, _scale))
	{
		scaleObj = rd.scale;
		setScale(rd.scale, rd.scale);
		origin = PosScale(positionObj.x, positionObj.y, rd.scale, rd.scale);
		setResize();
	}
	if (GETBIT(rd.bitMask, _size)) setCharacterSize(rd.size);
	if (GETBIT(rd.bitMask, _fontId)) setFont(*kernel.fonts[rd.fontId]);
	if (GETBIT(rd.bitMask, _text)) setString(sf::String::fromUtf8(rd.text.begin(), rd.text.end()));
	if (GETBIT(rd.bitMask, _color)) setColorText(rd.color, rd.alpha);
	if (GETBIT(rd.bitMask, _style)) setStyleText(rd.style);
	kernel.print("Edit mode for text: " + rd.id, INFO);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Text::display(sf::RenderWindow *win)
{
	win->draw(*this);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::ostream &ng::operator << (std::ostream& os, Text &t)
{
	return t.print(os);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::ostream & Text::print(std::ostream &os)
{
	sf::Vector2f pos = getPosition();
	sf::Vector2f scl = getScale();
	os << id << " [ng::Text]" << std::endl;
	os << "\tColor:   \t" << getFillColor().toInteger() << std::endl;
	os << "\tLayer:   \t" << layer << std::endl;
	os << "\tPosition:\t(" << pos.x << "; " << pos.y << ")" << std::endl;
	os << "\tScale:   \t(" << scl.x << "; " << scl.y << ")" << std::endl;
	return os;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Text::setResize()
{
	Displayable::setResize();
	setPosition(posScale.pos);
	setScale(posScale.scale);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Text::setLayerMotion()
{
	doLayerMotion(this);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~