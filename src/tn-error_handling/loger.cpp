#include "loger.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <format>
#include <filesystem>

using namespace error_handling;


loger::loger(std::string_view dir) {
    create_log(dir);
}

void loger::add_log(std::string msg, const error_code code) {
    auto now{std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now())};

    auto str_time{std::format("{:%T}", now)};

    auto log{std::format(
        "[{}]: {} | code -- {}\n",
        str_time, msg, static_cast<int>(code)
    )};


    std::fstream log_fp{logs_path, std::ios::app};

    if(!log_fp.is_open()) {
        std::cerr << "CAN'T OPEN THE LOG FILE. ERROR." << std::endl;

        return;
    }

    log_fp << log;

    log_fp.close();
}

void loger::create_log(std::string_view logs_dir) {
    std::filesystem::create_directories(logs_dir);

    auto local_time{std::chrono::current_zone()->to_local(std::chrono::system_clock::now())};
    
    std::filesystem::path dir{logs_dir};
    std::string filename = std::format("tn-log_{:%Y-%m-%d_%H-%M-%S}.log", local_time);
    
    logs_path = (dir / filename).string();

    std::ofstream log_fp{logs_path};

    if(!log_fp.is_open()) {
        std::cerr << "CAN'T CREATE THE LOG FILE. ERROR." << std::endl;

        return;
    }

    log_fp.close();
}