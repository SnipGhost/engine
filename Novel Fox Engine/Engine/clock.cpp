//-----------------------------------------------------------------------------
// clock.cpp                                        ���������� ������ clock.cpp
//-----------------------------------------------------------------------------
#include "engine.hpp"
//-----------------------------------------------------------------------------
using namespace ng;
//-----------------------------------------------------------------------------
int Clock::getMilliSecond()
{
	return this->getElapsedTime().asMilliseconds();
}
//-----------------------------------------------------------------------------