//-----------------------------------------------------------------------------
#include "engine.hpp"
#include <codecvt>
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Text::Text(ResData rd)
{
	setText(rd);
}
//-----------------------------------------------------------------------------
bool Text::setText(ResData &rd)
{
	(rd.namePerson != "") ? rd.text = rd.namePerson + ": " + rd.text : rd.text;
	setString(sf::String::fromUtf8(rd.text.begin(), rd.text.end()));
	setFont(*kernel.fonts[rd.fontId]);

	id = rd.id;

	mapping["red"] = 1;
	mapping["green"] = 2;
	mapping["blue"] = 3;
	mapping["yellow"] = 4;
	mapping["white"] = 5;
	mapping["black"] = 6;

	switch (mapping[rd.color])
	{
		case 1: setFillColor(sf::Color::Red);    break;
		case 2: setFillColor(sf::Color::Green);  break;
		case 3: setFillColor(sf::Color::Blue);   break;
		case 4: setFillColor(sf::Color::Yellow); break;
		case 5: setFillColor(sf::Color::White);  break;
		case 6: setFillColor(sf::Color::Black);  break;
	}

	setPosition(rd.x, rd.y);
	setCharacterSize(rd.size);

	return 1;
}
//-----------------------------------------------------------------------------
void Text::display(sf::RenderWindow *win)
{
	win->draw(*this);
}
//-----------------------------------------------------------------------------
std::ostream &ng::operator << (std::ostream& os, const Text &t)
{
	sf::Vector2f pos = t.getPosition();
	sf::Vector2f scl = t.getScale();
	os << t.id << " [ng::Text]" << std::endl;
	os << "\tColor:   \t" << t.getFillColor().toInteger() << std::endl;
	os << "\tLayer:   \t" << t.layer << std::endl;
	os << "\tPosition:\t(" << pos.x << "; " << pos.y << ")" << std::endl;
	os << "\tScale:   \t(" << scl.x << "; " << scl.y << ")" << std::endl;
	return os;
}
//-----------------------------------------------------------------------------