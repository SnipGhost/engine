#pragma once 
#ifndef LOAD_RES_HPP
#define LOAD_RES_HPP

#include <SFML/Graphics.hpp>
#include "tinyxml2.hpp"
#include <iostream>
#include <string>

tinyxml2::XMLElement* parseXML(std::string file);
tinyxml2::XMLElement* getSpriteXMLNode(tinyxml2::XMLElement* SPRITE);
void loadXMLComposer(std::string file);

#endif /* LOAD_RES_HPP */