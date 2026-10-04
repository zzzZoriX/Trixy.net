#pragma once

#include "tn-themes.hpp"
#include <string_view>
#include <string>


typedef struct settings {
    theme selected_theme;
    int ping_timeout;
    std::string logs_path;
    std::string config_path;

    settings(const int ping_timeout, const theme& theme, std::string_view logs_path, std::string_view config_path)
    :   ping_timeout(ping_timeout)
    ,   selected_theme(theme)
    ,   logs_path(logs_path)
    ,   config_path(config_path) {}
};