#include <tn-handlers/tn-config.hpp>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <array>
#include <filesystem>

using namespace handlers;


config::config(std::string_view config_file_path) 
    : config_file_path(config_file_path) {

    std::ifstream config_file{std::filesystem::path(config_file_path)};
    if (!config_file.is_open()) {
        throw std::runtime_error("Failed to open config file: " + std::string(config_file_path));
    }

    std::string line;
    while (std::getline(config_file, line)) {
        boost::trim(line);

        if (line.empty() || line.starts_with('#')) continue;

        auto delimiter_pos = line.find('=');
        if (delimiter_pos == std::string::npos) continue;

        std::string key = line.substr(0, delimiter_pos);
        std::string value = line.substr(delimiter_pos + 1);

        boost::trim(key);
        boost::trim(value);

        if (key == "settings") {
            config::files.settings_file = value;
        } else if (key == "servli") {
            config::files.servers_file = value;
        } else if (key == "logspath") {
            config::files.logs_path = value;
        }
    }
}

void config::commit() const {
    std::ofstream config_file(config_file_path);
    if (!config_file.is_open()) {
        throw std::runtime_error("Failed to open config file for writing: " + config_file_path);
    }

    config_file << "settings=" << files.settings_file << "\n";
    config_file << "servli=" << files.servers_file << "\n";
    config_file << "logspath=" << files.logs_path << "\n";

    config_file.close();
}

std::string config::get_logs_path() const {
    return files.logs_path;
}

std::string config::get_servers_file(void) const {
    return files.servers_file;
}

std::string config::get_settings_file(void) const {
    return files.settings_file;
}

void config::update_logs_path(const std::string&& logs_path) {
    files.logs_path = logs_path;
}

void config::update_servers_file(const std::string&& sfn) {
    files.servers_file = sfn;
}

void config::update_settings_file(const std::string&& stfn) {
    files.servers_file = stfn;
}