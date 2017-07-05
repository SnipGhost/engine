//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Text::Text(std::wstring text, float x, float y, int size, std::string path, std::string color)
{
	font.loadFromFile(path + "font.ttf");
	setString(text);
	setFont(font);

	mapping["red"   ] = 1;
	mapping["green" ] = 2;
	mapping["blue"  ] = 3;
	mapping["yellow"] = 4;
	mapping["white" ] = 5;
	mapping["black" ] = 6;

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
}
//-----------------------------------------------------------------------------
Text::Text(TextData td)
{
	//if (!setText(td.text, 0, 0, 39, td.color))
	//std::cout << "ErrorText " << td.text << std::endl;
}
//-----------------------------------------------------------------------------