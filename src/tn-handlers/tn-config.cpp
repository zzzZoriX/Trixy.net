#include <tn-handlers/tn-config.hpp>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <array>

using namespace handlers;


config::config(std::string_view config_file_path) 
:   config_file_path(config_file_path) {
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

config::~config() {
    std::ofstream config_file(config_file_path);
    if (!config_file.is_open()) {
        throw std::runtime_error("Failed to open config file for writing: " + config_file_path);
    }

    config_file << "settings=" << files.settings_file << "\n";
    config_file << "servli=" << files.servers_file << "\n";
    config_file << "logspath=" << files.logs_path << "\n";

    config_file.close();
}


std::vector<std::string> config::get_servers_list() const {
    std::ifstream slist_fp{files.servers_file};

    if(!slist_fp.is_open()) {
        throw std::runtime_error("Failed to open servers list file");
    }

    std::vector<std::string> servers_list;

    for(std::array<char, 65535> server; slist_fp.getline(&server[0], 65535, '\n');)
        servers_list.push_back(server.data());

    slist_fp.close();

    return servers_list;
}

std::string config::get_logs_path() const {
    return files.logs_path;
}

void config::add_server_to_list(const std::string& server) const {
    // todo: add validation for host name

    std::fstream slist_fp{files.servers_file, std::fstream::app};

    if(!slist_fp.is_open()) {
        throw std::runtime_error("Failed to open servers list file");
    }

    slist_fp.write(server.c_str(), server.size());
    slist_fp.put('\n');

    slist_fp.close();
}

void config::remove_server_from_list(const std::string& server) const {
    auto servers_list{get_servers_list()};

    if(auto i = std::find(servers_list.begin(), servers_list.end(), server); i != servers_list.end())
        servers_list.erase(i);


    std::ofstream slist_fp{files.servers_file};

    if(!slist_fp.is_open()) {
        throw std::runtime_error("Failed to open servers list file");
    }

    for(const auto& server: servers_list) {
        slist_fp.write(server.c_str(), server.size());
        slist_fp.put('\n');
    }
}

void config::update_logs_path(const std::string& logs_path) {
    files.logs_path = logs_path;
}