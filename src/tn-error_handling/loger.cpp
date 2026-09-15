#include "loger.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <format>

using namespace error_handling;


loger::loger(std::string_view path)
:   logs_path(path) {}

void loger::add_log(std::string msg, const error_code code) {
    auto now{std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now())};

    auto str_time{std::format("{:%T}", now)};

    session_logs.push_back(std::format(
        "[{}]: {} | code -- {}",
        str_time, msg, static_cast<int>(code)
    ));
}

void loger::create_log() const {
    auto local_time{std::chrono::current_zone()->to_local(std::chrono::system_clock::now())};


    std::ofstream log_fp{std::format("{}tn-log::{:%Y-%m-%d::%H:%M:%S}.log", logs_path, local_time)};

    if(!log_fp.is_open()) {
        std::cerr << "CAN'T CREATE THE LOG FILE. ERROR." << std::endl;

        return;
    }

    for(const auto& log: session_logs) {
        log_fp << log << "\n";
    }
    
    log_fp << "Program end successfully";

    log_fp.close();
}