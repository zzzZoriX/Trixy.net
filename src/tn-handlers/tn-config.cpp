#include <tn-handlers/tn-config.hpp>
#include <fstream>
#include <boost/algorithm/string.hpp>

using namespace handlers;


void config::load_config(std::string_view config_file_path) {
    std::ifstream config_file(config_file_path.data());
    if (!config_file.is_open()) {
        throw std::runtime_error("Failed to open config file: " + std::string(config_file_path));
    }

    std::string config_data((std::istreambuf_iterator<char>(config_file)), std::istreambuf_iterator<char>());
    config_file.close();

    std::vector<std::string> words;

    boost::split(words, config_data, boost::is_any_of("=\n"), boost::token_compress_on);

    for(auto i = 0; i < words.size(); ++i) {
        try {
            const auto& word = words.at(i++);

            if(word == "settings") {
                config::files.settings_file = words.at(i);
            }
            else if(word == "servli") {
                config::files.servers_file = words.at(i);
            }
            else if(word == "logspath") {
                config::files.logs_path = words.at(i);
            }
        }
        catch(const std::exception& e) {
            throw std::runtime_error("Error processing config data: " + std::string(e.what()));
        }
    }
}

void config::update_config() {
    std::ofstream config_file(config_file_path);
    if (!config_file.is_open()) {
        throw std::runtime_error("Failed to open config file for writing: " + config_file_path);
    }

    config_file << "settings=" << files.settings_file << "\n";
    config_file << "servli=" << files.servers_file << "\n";
    config_file << "logspath=" << files.logs_path << "\n";

    config_file.close();
}