//-----------------------------------------------------------------------------
// font.cpp                                           ���������� ������ �������
//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Font::Font(std::string src)
{
	id = "FONT";
	loadFromFile(src);
}
//-----------------------------------------------------------------------------
Font::Font(FontData fd)
{
	id = fd.id;
	loadFromFile(fd.src);
}
//-----------------------------------------------------------------------------