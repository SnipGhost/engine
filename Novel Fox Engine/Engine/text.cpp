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
	setText(rd);

	origin = PosScale(rd.x, rd.y, rd.scale, rd.scale);
	setResize();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Text::setText(ResData &rd)
{
	setString(sf::String::fromUtf8(rd.text.begin(), rd.text.end()));
	setFont(*kernel.fonts[rd.fontId]);

	id = rd.id;
	layer = rd.layer;
	layermotion = rd.layermotion;
	visible = rd.visible;
	positionObj = sf::Vector2f(rd.x, rd.y);
	scaleObj = rd.scale;

	//setOutlineThickness(2);				//Обводка
	//setOutlineColor(sf::Color::Blue);     //Цвет обводки

	setColorText(rd);
	setCharacterSize(rd.size);
	if (rd.style != "NULL") setStyleText(rd);

	return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Text::setColorText(ResData &rd)
{
	if (rd.color == "red")
		setFillColor(sf::Color::Red);
	if (rd.color == "green")
		setFillColor(sf::Color::Green);
	if (rd.color == "blue")
		setFillColor(sf::Color::Blue);
	if (rd.color == "yellow")
		setFillColor(sf::Color::Yellow);
	if (rd.color == "white")
		setFillColor(sf::Color::White);
	if (rd.color == "black")
		setFillColor(sf::Color::Black);

	//setColor(sf::Color(255, 255, 255, rd.alpha)); //Сделать установку alpha
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Text::setStyleText(ResData &rd)
{
	char *Ptr = strtok((char*)rd.style.c_str(), " ");

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
	//Layer - НЕЛЬЗЯ
	if (GETBIT(rd.bitMask, _x) && GETBIT(rd.bitMask, _y)) //ЕСЛИ ЕСТЬ ДВЕ КООРДИНАТЫ - ФИКСИТЬ
	{
		positionObj = sf::Vector2f(rd.x, rd.y); //Перезаписали стандартные значения
		setPosition(rd.x, rd.y);
		origin = PosScale(rd.x, rd.y, getScaleObj(), getScaleObj());
		setResize();
	}
	if (GETBIT(rd.bitMask, _scale))
	{
		scaleObj = rd.scale; //Перезаписали стандартные значения
		setScale(rd.scale, rd.scale);
		origin = PosScale(getPositionObj().x, getPositionObj().y, rd.scale, rd.scale);
		setResize();
	}
	if (GETBIT(rd.bitMask, _size)) setCharacterSize(rd.size);
	if (GETBIT(rd.bitMask, _fontId)) setFont(*kernel.fonts[rd.fontId]);
	if (GETBIT(rd.bitMask, _text)) setString(sf::String::fromUtf8(rd.text.begin(), rd.text.end()));
	if (GETBIT(rd.bitMask, _color)) setColorText(rd);
	if (GETBIT(rd.bitMask, _style)) setStyleText(rd);
	//if (GETBIT(rd.bitMask, _alpha)) setColor(sf::Color(255, 255, 255, rd.alpha));
	kernel.print("Edit mode for text: " + rd.id, INFO);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Text::display(sf::RenderWindow *win) //Переделать
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
	os << "\tReSize:  \t(" << kernel.factor.x << "; " << kernel.factor.y << ")" << std::endl;
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