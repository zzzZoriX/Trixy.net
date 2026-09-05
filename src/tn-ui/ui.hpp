#pragma once

#include "colorize.hpp"
#include <iostream>
#include <initializer_list>

namespace ui {

/**
 * @brief   colored print of text list
 * 
 * @param list  pair list of color and text
 */
void print(std::initializer_list<colorized_text> list);

}