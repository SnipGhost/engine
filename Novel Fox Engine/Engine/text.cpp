//-----------------------------------------------------------------------------
#include "engine.hpp"
#include <codecvt>
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Text::Text(std::string text, Font *font, std::string color, float x, float y, unsigned int size)
{
	TextData td = { "NONE", text, color, "UNKNOWN", font, size, 0, x, y };
	setText(td);
}
//-----------------------------------------------------------------------------
Text::Text(TextData td)
{
	setText(td);
}
//-----------------------------------------------------------------------------
bool Text::setText(TextData &td)
{
	setString(sf::String::fromUtf8(td.text.begin(), td.text.end()));
	setFont(*td.font);

	mapping["red"] = 1;
	mapping["green"] = 2;
	mapping["blue"] = 3;
	mapping["yellow"] = 4;
	mapping["white"] = 5;
	mapping["black"] = 6;

	switch (mapping[td.color])
	{
		case 1: setFillColor(sf::Color::Red);    break;
		case 2: setFillColor(sf::Color::Green);  break;
		case 3: setFillColor(sf::Color::Blue);   break;
		case 4: setFillColor(sf::Color::Yellow); break;
		case 5: setFillColor(sf::Color::White);  break;
		case 6: setFillColor(sf::Color::Black);  break;
	}

	setPosition(td.x, td.y);
	setCharacterSize(td.size);

	return 1;
}
//-----------------------------------------------------------------------------
void Text::display(sf::RenderWindow *win)
{
	win->draw(*this);
}
//-----------------------------------------------------------------------------