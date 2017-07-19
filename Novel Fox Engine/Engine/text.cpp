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
	/*computeLayerScale();*/ //Можно получить размер текста
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Text::setText(ResData &rd)
{
	(rd.namePerson != "") ? rd.text = rd.namePerson + ": " + rd.text : rd.text;
	setString(sf::String::fromUtf8(rd.text.begin(), rd.text.end()));
	setFont(*kernel.fonts[rd.fontId]);

	id = rd.id;
	layer = rd.layer;
	visible = rd.visible;

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

	if (rd.style != "NULL") setStyleText(rd);

	setPosition(rd.x, rd.y);
	setCharacterSize(rd.size);
	setScale(rd.scale, rd.scale);

	posScale = ng::setResize(this);

	return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Text::setStyleText(ResData &rd)
{
	char *Ptr = strtok((char*)rd.style.c_str() , " ");

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
void Text::computeLayerScale()
{
	float w = this->getGlobalBounds().width;        // Бывает неадекватно [!]
	float h = this->getGlobalBounds().height;
	//// ВНИМАНИЕ: константа выведена эмпирически, значит это лишь частный случай
	//// Рассчитываем scale в зависимости от слоя (квадратичная зависимость)
	float sx = posScale.scale.x + (float)0.03 * (2 << (layer - 1));
	float sy = posScale.scale.y + (float)0.03 * (2 << (layer - 1));
	//// Теперь выставляем новые координаты за вычетом увеличения пополам
	posScale.pos.x = posScale.pos.x - w * (sx - posScale.scale.x) / 2;
	posScale.pos.y = posScale.pos.y - h * (sy - posScale.scale.y) / 2;
	//// Ну и потом можно увеличить (теперь уже в зависимости от Layer)

	std::cout << w << " " << h << std::endl;

	setScale(sx, sy);
}