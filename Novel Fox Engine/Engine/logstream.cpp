//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// logstream.cpp                                        Реализация класса логов
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "engine.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace ng;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LogStream::LogStream(unsigned int mask)
{
	log = &std::cout;
	tag_mask = mask;
	isExtOS = true;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LogStream::LogStream(std::ostream &os, unsigned int mask)
{
	log = &os;
	isExtOS = true;
	tag_mask = mask;
	check();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LogStream::LogStream(std::string file, unsigned int mask)
{
	tag_mask = mask;
	if (strcmp(file.c_str(), "std::cout") == 0)
	{
		log = &std::cout;
		isExtOS = true;
	}
	else
	{
		log = new std::ofstream(file);
		isExtOS = false;
		check();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LogStream::~LogStream()
{
	if (!isExtOS)
	{
		((std::ofstream*)log)->close();
		delete log;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool LogStream::check()
{
	if (log->fail())
	{
		if (!isExtOS) delete log;
		log = &std::cout;
		std::cout << "Error opening ostream!" << std::endl;
		return 0;
	}
	return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void LogStream::setTagMask(unsigned int mask)
{
	tag_mask = mask;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~