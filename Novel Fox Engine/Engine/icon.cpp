#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
Icon::Icon(std::string src)
{
	if (!setIcon(src))
		std::cout << "Failed load icon " << src << std::endl;
	else
		std::cout << "Set icon " << src << std::endl;
}
//-----------------------------------------------------------------------------
bool Icon::setIcon(std::string src)
{
	if (!loadFromFile(src)) return 0;
		return 1;
}
//-----------------------------------------------------------------------------