#pragma once

#include <string_view>
#include <string>
#include <vector>
#include "error_codes.hpp"


namespace error_handling {

class loger {
    std::vector<std::string> session_logs;
    std::string logs_path;

public:
    loger(std::string_view path);

    void add_log(std::string msg, const error_code code);

    void create_log() const;
};

}