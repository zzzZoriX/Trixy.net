#include "tn-themes.hpp"

void theme::set_theme(std::string_view theme) {
    if(theme == "purple")
        load(theme_purple);
    else if(theme == "crimson")
        load(theme_crimson);
    else if(theme == "blue")
        load(theme_blue);
    else if(theme == "green")
        load(theme_green);
    else if(theme == "teal")
        load(theme_teal);
    else if(theme == "white")
        load(theme_white);
    else if(theme == "black")
        load(theme_black);
    else if(theme == "monochrome")
        load(theme_monochrome);
}

void theme::load(theme t) {
    for(int i = 0; i < 3; ++i) 
        paragraph[i] = t.paragraph[i];
    
    for(int i = 0; i < 3; ++i) 
        text[i] = t.text[i];
    
    for(int i = 0; i < 3; ++i) 
        borders[i] = t.borders[i];
    
    for(int i = 0; i < 3; ++i) 
        selected[i] = t.selected[i];
    
    for(int i = 0; i < 3; ++i) 
        buttons_text[i] = t.buttons_text[i];
}

std::string theme::convert(theme t) {
    if(theme_crimson == t) return "crimson";
    if(theme_black == t) return "black";
    if(theme_blue == t) return "blue";
    if(theme_green == t) return "green";
    if(theme_monochrome == t) return "mono";
    if(theme_purple == t) return "purple";
    if(theme_teal == t) return "teal";
    if(theme_white == t) return "white";

    return "default";
}
