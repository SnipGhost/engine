#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Text::Text(std::string text, sf::Vector2f vector, std::string srcFont, sf::Color color)
{
	if (!setText(text, vector, srcFont, color))
	std::cout << "ErrorText " << text << std::endl;
}
//-----------------------------------------------------------------------------
//Text::Text(TextData td)
//{
//	if (!setText(td.text, td.volume))
//	std::cout << "ErrorText " << td.text << std::endl;
//}
//-----------------------------------------------------------------------------
bool Text::setText(std::string text, sf::Vector2f vector, std::string srcFont, sf::Color color)
{
	if (!font.loadFromFile(srcFont)) return 0;
	setString(text);
	setFont(font);
	setFillColor(color);
	setPosition(vector.x, vector.y);
	return 1;
}
//-----------------------------------------------------------------------------