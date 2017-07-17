//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// text.cpp                                            Реализация класса текста
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
#include <codecvt>
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
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Text::setText(ResData &rd)
{
	(rd.namePerson != "") ? rd.text = rd.namePerson + ": " + rd.text : rd.text;
	setString(sf::String::fromUtf8(rd.text.begin(), rd.text.end()));
	setFont(*kernel.fonts[rd.fontId]);

	id = rd.id;
	layer = rd.layer;

	if (rd.color == "red") setFillColor(sf::Color::Red);
	if (rd.color == "green") setFillColor(sf::Color::Green);
	if (rd.color == "blue") setFillColor(sf::Color::Blue);
	if (rd.color == "yellow") setFillColor(sf::Color::Yellow);
	if (rd.color == "white") setFillColor(sf::Color::White); 
	if (rd.color == "black") setFillColor(sf::Color::Black);

	if (rd.style != "NULL") setStyleText(rd);

	setPosition(rd.x, rd.y);
	setCharacterSize(rd.size);
	setScale(rd.scale, rd.scale);

	posScale = ng::setResize(this);

	return 1;
}
void Text::setStyleText(ResData &rd)
{
	//char *Ptr = strtok((char*)rd.style.c_str() , " ");

	//std::string styleNum = "";

	//while (Ptr)
	//{
	//	if (!strcmp(Ptr, "bold"))
	//		styleNum += BOLD;
	//	if (!strcmp(Ptr, "strikethrough"))
	//		styleNum += "2";
	//	if (!strcmp(Ptr, "italic"))
	//		styleNum += "3";
	//	if (!strcmp(Ptr, "underlined")) 
	//		styleNum += "4";

	//	setStyle(styleNum);

	//	Ptr = strtok(0, " ");
	//}

	if (rd.style == "bold") setStyle(BOLD);
	if (rd.style == "strikethrough") setStyle(STRIKETHROUGH);
	if (rd.style == "italic") setStyle(ITALIC);
	if (rd.style == "underlined") setStyle(UNDERLINED);
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
	os << "\tReSize:  \t(" << KWS_X << "; " << KWS_Y << ")" << std::endl;
	return os;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Text::setResize()
{
	posScale = ng::setResize(this);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Text::setLayerMotion()
{
	doLayerMotion(this);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~