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
//std::wstring wide_string(std::string const &s, std::locale const &loc)
//{
//	if (s.empty())
//		return std::wstring();
//	std::ctype<wchar_t> const &facet = std::use_facet<std::ctype<wchar_t> >(loc);
//	char const *first = s.c_str();
//	char const *last = first + s.size();
//	std::vector<wchar_t> result(s.size());
//
//	facet.widen(first, last, &result[0]);
//
//	return std::wstring(result.begin(), result.end());
//}
//-----------------------------------------------------------------------------
//std::wstring char2wchar(const std::string & p_in, const std::locale & p_loc)
//{
//	try {
//		std::wstring out(p_in.length(), 0);
//		std::string::const_iterator i = p_in.begin(), ie = p_in.end();
//		std::wstring::iterator j = out.begin();
//		for (; i != ie; ++i, ++j)
//			*j = std::use_facet< std::ctype< wchar_t > >(p_loc).widen(*i);
//		return out;
//	}
//	catch (...)
//	{
//		return std::wstring();
//	}
//};
//-----------------------------------------------------------------------------
bool Text::setText(std::string text, std::string color, float x, float y, unsigned int size)
{
	font.loadFromFile("Resources/font.ttf");

	/*char const *russian_locale_designator = "rus";
	std::locale loc(russian_locale_designator);
	std::wstring const textR = char2wchar(text, loc);
	std::locale::global(loc);*/

	//std::string str(text);
	//std::wstring wstr(str.begin(), str.end());

	setString(text); //_T _TEXT !!!
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