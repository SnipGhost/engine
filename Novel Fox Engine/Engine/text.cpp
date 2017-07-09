//-----------------------------------------------------------------------------
#include "engine.hpp"
#include <codecvt>
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Text::Text(std::string text, std::string color, float x, float y, unsigned int size)
{
	setText(text, color, x, y, size);
}
//-----------------------------------------------------------------------------
Text::Text(TextData td)
{
	setText(td.text, td.color, td.x, td.y, td.size);
}
//-----------------------------------------------------------------------------
bool Text::setText(std::string text, std::string color, float x, float y, unsigned int size)
{
	font.loadFromFile(RES_PATH + "font.ttf");

	setString(sf::String::fromUtf8(text.begin(), text.end()));
	setFont(font);

	mapping["red"] = 1;
	mapping["green"] = 2;
	mapping["blue"] = 3;
	mapping["yellow"] = 4;
	mapping["white"] = 5;
	mapping["black"] = 6;

	switch (mapping[color])
	{
		case 1: setFillColor(sf::Color::Red);    break;
		case 2: setFillColor(sf::Color::Green);  break;
		case 3: setFillColor(sf::Color::Blue);   break;
		case 4: setFillColor(sf::Color::Yellow); break;
		case 5: setFillColor(sf::Color::White);  break;
		case 6: setFillColor(sf::Color::Black);  break;
	}

	setPosition(x, y);
	setCharacterSize(size);

	return 1;
}
//-----------------------------------------------------------------------------
void Text::display(sf::RenderWindow *win)
{
	win->draw(*this);
}
//-----------------------------------------------------------------------------