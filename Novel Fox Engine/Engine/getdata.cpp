//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// getdata.cpp                       Получение и возврат информации по ресурсам
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Получение и возврат информации по ресурсам
ResData ng::getData(XMLNode node)
{
	const char *id = node->Attribute("id");
	const char *x = node->Attribute("x");
	const char *y = node->Attribute("y");
	const char *layermotion = node->Attribute("layermotion");
	const char *scale = node->Attribute("scale");
	const char *angle = node->Attribute("angle");
	const char *layer = node->Attribute("layer");
	const char *style = node->Attribute("style");
	const char *visible = node->Attribute("visible");
	const char *alpha = node->Attribute("alpha");
	const char *src = node->Attribute("src");
	const char *smooth = node->Attribute("smooth");

	const char *width = node->Attribute("width");
	const char *height = node->Attribute("height");
	const char *loop = node->Attribute("loop");
	const char *delay = node->Attribute("delay");

	const char *command = node->Attribute("command");
	const char *time = node->Attribute("time");
	const char *volume = node->Attribute("volume");

	const char *size = node->Attribute("size");
	const char *text = node->GetText();
	const char *fontId = node->Attribute("font");
	const char *color = node->Attribute("color");

	ResData res;
	res.bitMask = 0;
	if (id) { res.id = id; res.bitMask = res.bitMask | (1 << _id); } else res.id = "NULL";
	if (x) { res.x = std::stof(x); res.bitMask = res.bitMask | (1 << _x); } else res.x = 0;
	if (y) { res.y = std::stof(y); res.bitMask = res.bitMask | (1 << _y); } else res.y = 0;
	if (layermotion) { res.layermotion = CONVTRUE(layermotion); res.bitMask = res.bitMask | (1 << _layermotion); } else res.layermotion = true;
	if (scale) { res.scale = std::stof(scale); res.bitMask = res.bitMask | (1 << _scale); } else res.scale = 1;
	if (angle) { res.angle = std::stof(angle); res.bitMask = res.bitMask | (1 << _angle); } else res.angle = 0;
	if (layer) { res.layer = std::atoi(layer); res.bitMask = res.bitMask | (1 << _layer); } else res.layer = 0;
	if (style) { res.style = style; res.bitMask = res.bitMask | (1 << _style); } else res.style = "NULL";
	if (visible) { res.visible = CONVTRUE(visible); res.bitMask = res.bitMask | (1 << _visible); } else res.visible = true;
	if (alpha) { res.alpha = 255 * std::atoi(alpha) / 100; res.bitMask = res.bitMask | (1 << _alpha); } else res.alpha = 255;
	if (src) { res.src = RES_PATH + std::string(src); res.bitMask = res.bitMask | (1 << _src); } else res.src = "NULL";
	if (smooth) { res.smooth = CONVTRUE(smooth); res.bitMask = res.bitMask | (1 << _smooth); } else res.smooth = true;

	if (width) { res.width = std::atoi(width); res.bitMask = res.bitMask | (1 << _width); } else res.width = 0;
	if (height) { res.height = std::atoi(height); res.bitMask = res.bitMask | (1 << _height); } else res.height = 0;
	if (loop) { res.loop = CONVTRUE(loop); res.bitMask = res.bitMask | (1 << _loop); } else res.loop = false;
	if (delay) { res.delay = std::atoi(delay); res.bitMask = res.bitMask | (1 << _delay); } else res.delay = 40;

	if (command) { res.command = command; res.bitMask = res.bitMask | (1 << _command); } else res.command = "NULL";
	if (time) { res.time = std::atoi(time); res.bitMask = res.bitMask | (1 << _time); } else res.time = 1000;
	if (volume) { res.volume = std::stof(volume); res.bitMask = res.bitMask | (1 << _volume); } else res.volume = 0;

	if (size) { res.size = std::atoi(size); res.bitMask = res.bitMask | (1 << _size); } else res.size = 1;
	if (text) { res.text = text; res.bitMask = res.bitMask | (1 << _text); } else res.text = "NO TEXT";
	if (fontId) { res.fontId = fontId; res.bitMask = res.bitMask | (1 << _fontId); } else res.fontId = "standart";
	if (color) { res.color = color; res.bitMask = res.bitMask | (1 << _color); } else res.color = "black";

	return res;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Получение и возврат информации по шрифтам
FontData ng::getFontData(XMLNode node)
{
	const char *src = node->Attribute("src");
	const char *id = node->Attribute("id");

	FontData res;
	(id) ? res.id = id : res.id = "NULL";
	(src) ? res.src = RES_PATH + std::string(src) : res.src = "NULL";
	return res;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~