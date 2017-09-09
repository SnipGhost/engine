//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// smarttext.cpp     Реализация класса умного текста с автоматическим переносом
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SmartText::SmartText(ResData rd)
{
	id = rd.id;
	layer = rd.layer;
	layermotion = rd.layermotion;
	visible = rd.visible;
	positionObj = sf::Vector2f(rd.x, rd.y);
	scaleObj = rd.scale;
	fontId = rd.fontId;
	size = rd.size;
	color = rd.color;
	alpha = rd.alpha;
	style = rd.style;
	width = rd.width;
	height = rd.height;
	text = rd.text;

	setSmartText(rd);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SmartText::setSmartText(ResData &rd)
{
	textExample = new Text(id, layer, text, fontId, layermotion, visible, positionObj.x,
		positionObj.y, scaleObj, size, color, alpha, style);

	interval = textExample->getLocalBounds().height + height;

	int line = 0;
	bool division = true;
	std::string textString = text;
	size_t len_sym = (size_t)textString.length(); // Длина строки в символах
	size_t len_pix = (size_t)textExample->getLocalBounds().width; //длина строки в пикселях
	size_t avg_sym_size = len_pix / len_sym; // средняя длина символа в пикселях
	size_t line_end_pos = width / avg_sym_size; // примерное кол-во символов которое влезет в строку, округлим в малую сторону
	while (division == true)
	{
		len_sym = textString.length(); // Обновляем длину строки в символах
		if (line_end_pos < len_sym) // Строка больше ограничения, значит делим
		{
			for (int i = line_end_pos; i > 0; i--)
			{
				if (textString[i] == ' ')
				{
					textExample = new Text("line" + std::to_string(line), layer, textString.substr(0, i), fontId, layermotion,
						visible, positionObj.x, positionObj.y + interval * line, scaleObj, size,
						color, alpha, style);
					line++;
					textVector.push_back(textExample);
					textString = textString.substr(i+1, len_sym - i);
					break;
				}
			}
		}
		else // Последняя строка, что меньше ограничения
		{
			textExample = new Text("line" + std::to_string(line), layer, textString, fontId, layermotion,
				visible, positionObj.x, positionObj.y + interval * line, scaleObj, size,
				color, alpha, style);
			textVector.push_back(textExample);
			division = false;
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool SmartText::isMouseAbove()
{ 
	for (auto &text : textVector)
	{
		if (text->isMouseAbove())
		{
			return 1;
		}
	}
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SmartText::edit(ResData rd) //Доработать
{
	if (GETBIT(rd.bitMask, _x) || GETBIT(rd.bitMask, _y) || 
		GETBIT(rd.bitMask, _scale) || GETBIT(rd.bitMask, _text))
	{
		if (GETBIT(rd.bitMask, _x))
			positionObj.x = rd.x;
		if (GETBIT(rd.bitMask, _y))
			positionObj.y = rd.y;
		if (GETBIT(rd.bitMask, _scale))
			scaleObj = rd.scale;
		if (GETBIT(rd.bitMask, _text))
			text = rd.text;
		textVector.clear();
		setSmartText(rd);
	}
	if (GETBIT(rd.bitMask, _size))
	{
		size = rd.size;
		for (auto &text : textVector)
			text->setCharacterSize(rd.size);
	}
	if (GETBIT(rd.bitMask, _fontId))
	{
		fontId = rd.fontId;
		for (auto &text : textVector)
			text->setFont(*kernel.fonts[rd.fontId]);
	}
	if (GETBIT(rd.bitMask, _color))
	{
		color = rd.color;
		for (auto &text : textVector)
			text->setColorText(rd.color, rd.alpha);
	}
	if (GETBIT(rd.bitMask, _style))
	{
		style = rd.style;
		for (auto &text : textVector)
			text->setStyleText(rd.style);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SmartText::display(sf::RenderWindow *win)
{
	for (auto &text : textVector)
	{
		text->display(win);
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::ostream &ng::operator << (std::ostream& os, SmartText &t)
{
	return t.print(os);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::ostream & SmartText::print(std::ostream &os)
{
	//Большинство информации спрашиваем у первой строчки
	sf::Vector2f pos = textVector[0]->getPosition();
	sf::Vector2f scl = textVector[0]->getScale();
	os << id << " [ng::SmartText]" << std::endl;
	os << "\tColor:   \t" << textVector[0]->getFillColor().toInteger() << std::endl;
	os << "\tLayer:   \t" << layer << std::endl;
	os << "\tPosition:\t(" << pos.x << "; " << pos.y << ")" << std::endl;
	os << "\tScale:   \t(" << scl.x << "; " << scl.y << ")" << std::endl;
	return os;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SmartText::setResize()
{
	for (auto &text : textVector)
	{
		text->setResize();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SmartText::setLayerMotion()
{
	for (auto &text : textVector)
	{
		text->setLayerMotion();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~