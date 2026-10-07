#pragma once

#include <ftxui/ftxui.hpp>
#include <string_view>
#include <array>


struct theme {
    std::array<unsigned char, 3> paragraph;
    std::array<unsigned char, 3> text;
    std::array<unsigned char, 3> borders;
    std::array<unsigned char, 3> selected;

    void set_theme(std::string_view t);
    void load(theme t);

    static std::string convert(theme t);

    bool operator==(theme& other) const {
        bool p = false, 
             t = false,
             b = false, 
             s = false;

        int tc{0};

        for(int i = 0; i < 3; ++i)
           if(paragraph[i] == other.paragraph[i]) ++tc;
        if(tc == 3) p = true; 

        tc = 0;

        for(int i = 0; i < 3; ++i)
           if(text[i] == other.text[i]) ++tc;
        if(tc == 3) t = true; 

        tc = 0;

        for(int i = 0; i < 3; ++i)
           if(borders[i] == other.borders[i]) ++tc;
        if(tc == 3) b = true; 

        tc = 0;

        for(int i = 0; i < 3; ++i)
           if(selected[i] == other.selected[i]) ++tc;
        if(tc == 3) s = true; 

        return s && b && t && p;
    }
};

const std::vector<std::string> themes_names = {
    "purple", "crimson", "blue", "green", "teal", "black", "white", "monochrome"
};

constexpr theme theme_purple = {
    .paragraph    = {186, 85, 211},  
    .text         = {230, 230, 250},
    .borders      = {138, 43, 226},
    .selected     = {218, 112, 214}
};

constexpr theme theme_crimson = {
    .paragraph    = {220, 20, 60},
    .text         = {255, 129, 129},
    .borders      = {139, 0, 0},
    .selected     = {189, 0, 0}
};

constexpr theme theme_blue = {
    .paragraph    = {102, 215, 255},
    .text         = {220, 235, 252},
    .borders      = {30, 144, 255},
    .selected     = {0, 255, 255}
};

constexpr theme theme_green = {
    .paragraph    = {30, 233, 30},
    .text         = {220, 255, 220},
    .borders      = {0, 128, 0},
    .selected     = {0, 255, 127}
};

constexpr theme theme_teal = {
    .paragraph    = {0, 229, 238},
    .text         = {224, 255, 255},
    .borders      = {0, 139, 139},
    .selected     = {64, 224, 208}
};

constexpr theme theme_white = {
    .paragraph    = {30, 144, 255},
    .text         = {20, 20, 20},
    .borders      = {180, 180, 180},
    .selected     = {70, 130, 180}
};

constexpr theme theme_black = {
    .paragraph    = {0, 179, 255},
    .text         = {200, 200, 200},
    .borders      = {60, 60, 60},
    .selected     = {255, 255, 255}
};

constexpr theme theme_monochrome = {
    .paragraph    = {255, 255, 255},
    .text         = {200, 200, 200},
    .borders      = {100, 100, 100},
    .selected     = {255, 255, 255}
};
