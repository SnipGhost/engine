//-----------------------------------------------------------------------------
#include "engine.hpp"
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
std::wstring wide_string(std::string const &s, std::locale const &loc)
{
	if (s.empty())
		return std::wstring();
	std::ctype<wchar_t> const &facet = std::use_facet<std::ctype<wchar_t> >(loc);
	char const *first = s.c_str();
	char const *last = first + s.size();
	std::vector<wchar_t> result(s.size());

	facet.widen(first, last, &result[0]);

	return std::wstring(result.begin(), result.end());
}
//-----------------------------------------------------------------------------
bool Text::setText(std::string text, std::string color, float x, float y, unsigned int size)
{
	font.loadFromFile("Resources/font.ttf");

	std::locale loc("rus");
	std::wstring const textR = wide_string(text, loc);
	std::locale::global(loc);

	setString(textR);
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