#include "ui.hpp"
#include <vector>

void ui::print(std::initializer_list<ui::colorized_text> list) {
    std::vector<ui::colorized_text> pair_list{list};

    for(const auto& [color, text]: pair_list) {
        std::cout << color << text;
    }

    std::cout << RESET;
}