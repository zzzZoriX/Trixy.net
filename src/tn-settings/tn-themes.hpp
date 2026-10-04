#pragma once

#include <ftxui/ftxui.hpp>

typedef struct theme {
    unsigned char paragraph[3];
    unsigned char text[3];
    unsigned char borders[3];
    unsigned char buttons_text[3];
    unsigned char selected[3];
};

constexpr theme theme_purple = {
    .paragraph    = {186, 85, 211},  // Medium Orchid
    .text         = {230, 230, 250},  // Lavender
    .borders      = {138, 43, 226},   // Blue Violet
    .buttons_text = {255, 255, 255},  // White
    .selected     = {218, 112, 214}   // Orchid
};

constexpr theme theme_crimson = {
    .paragraph    = {220, 20, 60},    // Crimson
    .text         = {245, 245, 245},  // White Smoke
    .borders      = {139, 0, 0},      // Dark Red
    .buttons_text = {255, 215, 0},    // Gold
    .selected     = {255, 69, 0}      // Orange Red
};

constexpr theme theme_blue = {
    .paragraph    = {0, 191, 255},    // Deep Sky Blue
    .text         = {220, 235, 252},  // Ice Blue
    .borders      = {30, 144, 255},   // Dodger Blue
    .buttons_text = {255, 255, 255},  // White
    .selected     = {0, 255, 255}     // Cyan
};

constexpr theme theme_green = {
    .paragraph    = {50, 205, 50},    // Lime Green
    .text         = {220, 255, 220},  // Pale Green
    .borders      = {0, 128, 0},      // Green
    .buttons_text = {255, 255, 255},  // White
    .selected     = {0, 255, 127}     // Spring Green
};

constexpr theme theme_teal = {
    .paragraph    = {0, 229, 238},    // Cyan / Turquoise
    .text         = {224, 255, 255},  // Light Cyan
    .borders      = {0, 139, 139},    // Dark Cyan
    .buttons_text = {255, 255, 255},  // White
    .selected     = {64, 224, 208}    // Turquoise
};

constexpr theme theme_white = {
    .paragraph    = {30, 144, 255},   // Dodger Blue Accent
    .text         = {20, 20, 20},     // Dark Charcoal Text
    .borders      = {180, 180, 180},  // Light Gray
    .buttons_text = {0, 0, 0},        // Black
    .selected     = {70, 130, 180}    // Steel Blue
};

constexpr theme theme_black = {
    .paragraph    = {0, 179, 255},    // Neon Cyan Accent
    .text         = {200, 200, 200},  // Off-white
    .borders      = {60, 60, 60},     // Dark Gray
    .buttons_text = {255, 255, 255},  // White
    .selected     = {255, 255, 255}   // High Contrast White
};

constexpr theme theme_monochrome = {
    .paragraph    = {255, 255, 255},  // Pure White
    .text         = {200, 200, 200},  // Light Gray
    .borders      = {100, 100, 100},  // Mid Gray
    .buttons_text = {220, 220, 220},  // Light Gray
    .selected     = {255, 255, 255}   // Inverted / Highlight
};
