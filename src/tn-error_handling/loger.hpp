#pragma once

#include <string_view>
#include <string>
#include <vector>
#include "error_codes.hpp"


namespace error_handling {

class loger {
    std::string logs_path;

public:
    loger(std::string_view path);

    void add_log(std::string msg, const error_code code);

private:
    void create_log(std::string_view logs_dir);
};

}