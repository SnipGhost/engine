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
	setText(rd.id, rd.layer, rd.text, rd.fontId, rd.layermotion, rd.visible, 
		    rd.x, rd.y, rd.scale, rd.size, rd.color, rd.alpha, rd.style, rd.angle);

	origin = PosScale(rd.x, rd.y, rd.scale, rd.scale);
	setResize();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Text::Text(std::string id, int layer, std::string text, std::string fontId,
bool layermotion, bool visible, float x, float y, float scale, unsigned int _size, 
std::string color, int alpha, std::string style, float angle)
{
	setText(id, layer, text, fontId, layermotion, visible, x, y, scale, _size, color, alpha, style, angle);

	origin = PosScale(x, y, scale, scale);
	setResize();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Text::setText(std::string _id, int _layer, std::string text, std::string fontId,
bool _layermotion, bool _visible, float x, float y, float scale, unsigned int _size, 
std::string color, int alpha, std::string style, float angle)
{
	setString(sf::String::fromUtf8(text.begin(), text.end()));
	setFont(*kernel.fonts[fontId]);

	setRotation(angle);

	id = _id;
	layer = _layer;
	layermotion = _layermotion;
	visible = _visible;
	positionObj = sf::Vector2f(x, y);
	scaleObj = scale;
	size = _size;

	setCharacterSize(size);
	setColorText(color, alpha);
	setStyleText(style);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Text::setColorText(std::string color, int alpha)
{

	if (color == "black")
	{
		setFillColor(sf::Color(0, 0, 0, alpha));
		return;
	}
	if (color == "white")
	{
		setFillColor(sf::Color(255, 255, 255, alpha));
		return;
	}
	if (color == "red")
	{
		setFillColor(sf::Color(255, 0, 0, alpha));
		return;
	}
	if (color == "green")
	{
		setFillColor(sf::Color(0, 128, 0, alpha));
		return;
	}
	if (color == "blue")
	{
		setFillColor(sf::Color(0, 0, 255, alpha));
		return;
	}
	if (color == "yellow")
	{
		setFillColor(sf::Color(255, 255, 0, alpha));
		return;
	}

	// Установка RGB цвета
	char *now = strtok((char*)color.c_str(), " .,:");
	int red = std::atoi(now);
	now = strtok(0, " .,:");
	int green = std::atoi(now);
	now = strtok(0, " .,:");
	int blue = std::atoi(now);

	setFillColor(sf::Color(red, green, blue, alpha));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Text::setStyleText(std::string style)
{
	char *ptr = strtok((char*)style.c_str(), " .,:");

	unsigned int styleNum = 0;

	while (ptr)
	{
		if (!strcmp(ptr, "bold"))
			styleNum += BOLD;
		if (!strcmp(ptr, "strikethrough"))
			styleNum += STRIKETHROUGH;
		if (!strcmp(ptr, "italic"))
			styleNum += ITALIC;
		if (!strcmp(ptr, "underlined"))
			styleNum += UNDERLINED;
		//if (!strcmp(ptr, "outline")) //В поле Style: outline.0.0.0.255.2
		//{
		//	ptr = strtok(0, " .,:");
		//	int red = std::atoi(ptr);
		//	ptr = strtok(0, " .,:");
		//	int green = std::atoi(ptr);
		//	ptr = strtok(0, " .,:");
		//	int blue = std::atoi(ptr);
		//	ptr = strtok(0, " .,:");
		//	int alpha = std::atoi(ptr);
		//	ptr = strtok(0, " .,:");
		//	int thickness = std::atoi(ptr);

		//	setOutlineThickness(thickness);
		//	setOutlineColor(sf::Color(red, green, blue, alpha));

		//}
		ptr = strtok(0, " .,:");
	}

	setStyle(styleNum);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Text::isMouseAbove()
{
	float x_indent = getLocalBounds().left * posScale.scale.x;
	float y_indent = getLocalBounds().top * posScale.scale.y;

	float x_circuit = getLocalBounds().width * posScale.scale.x;
	float y_circuit = getLocalBounds().height * posScale.scale.y;

	if (kernel.getMouse().x >= (posScale.pos.x + x_indent) &&
		kernel.getMouse().x < (posScale.pos.x + x_circuit + x_indent) &&
		kernel.getMouse().y >= (posScale.pos.y + y_indent) &&
		kernel.getMouse().y < (posScale.pos.y + y_circuit + y_indent))
	{
		return 1;
	}
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Text::edit(ResData rd)
{
	if (GETBIT(rd.bitMask, _visible)) 
	{
		visible = rd.visible;
	}
	if (GETBIT(rd.bitMask, _x) || GETBIT(rd.bitMask, _y))
	{
		if (GETBIT(rd.bitMask, _x))
		{
			positionObj.x = rd.x;
			setPosition(rd.x, positionObj.y);
			origin = PosScale(rd.x, positionObj.y, scaleObj, scaleObj);
		}
		if (GETBIT(rd.bitMask, _y))
		{
			positionObj.y = rd.y;
			setPosition(positionObj.x, rd.y);
			origin = PosScale(positionObj.x, rd.y, scaleObj, scaleObj);
		}
		setResize();
	}
	if (GETBIT(rd.bitMask, _scale))
	{
		scaleObj = rd.scale;
		setScale(rd.scale, rd.scale);
		origin = PosScale(positionObj.x, positionObj.y, rd.scale, rd.scale);
		setResize();
	}
	if (GETBIT(rd.bitMask, _size))
	{
		size = rd.size;
		setCharacterSize(rd.size);
	}
	if (GETBIT(rd.bitMask, _fontId))
	{
		fontId = rd.fontId;
		setFont(*kernel.fonts[rd.fontId]);
	}
	if (GETBIT(rd.bitMask, _text))
	{
		setString(sf::String::fromUtf8(rd.text.begin(), rd.text.end()));
	}
	if (GETBIT(rd.bitMask, _color))
	{
		color = rd.color;
		setColorText(rd.color, alpha);
	}
	if (GETBIT(rd.bitMask, _alpha))
	{
		alpha = rd.alpha;
		setColorText(color, rd.alpha);
	}
	if (GETBIT(rd.bitMask, _style))
	{
		style = rd.style;
		setStyleText(rd.style);
	}
	if (GETBIT(rd.bitMask, _angle))
	{
		angle = rd.angle;
		setRotation(rd.angle);
	}
	kernel.print("Edit mode for text: " + rd.id, INFO);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Text::display(sf::RenderWindow *win)
{
	win->draw(*this);
	if (this->isMouseAbove()) // ТЕСТ [!]
	{
		outlineShape.setOutlineShape(sf::Vector2f(getLocalBounds().width * posScale.scale.x, getLocalBounds().height * posScale.scale.y), sf::Vector2f(posScale.pos.x + getLocalBounds().left  * posScale.scale.x, posScale.pos.y + getLocalBounds().top * posScale.scale.y));
		win->draw(outlineShape);
	}
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