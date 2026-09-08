#include "tn-servers.hpp"
#include <fstream>
#include <array>
#include <algorithm>

using namespace handlers;


servers::servers(std::string_view slist_fn)
:   slist_fn(slist_fn) {
    std::ifstream slist_fp{slist_fn.data()};

    if(!slist_fp.is_open()) {
        throw std::runtime_error("Failed to open servers list file");
    }

    std::vector<std::string> servers_list;

    for(std::array<char, 65535> server; slist_fp.getline(&server[0], 65535, '\n');)
        servers_list.push_back(server.data());

    slist_fp.close();
}

servers::~servers() {
    std::ofstream slist_fp{slist_fn};

    if(!slist_fp.is_open()) {
        throw std::runtime_error("Failed to open servers list file");
    }

    for(const auto& server: servers_list) {
        slist_fp.write(server.c_str(), server.size());
        slist_fp.put('\n');
    }
}

void servers::add_server_to_list(const std::string server) {
    if(!validate_server_name(server)) {
        throw std::invalid_argument("Invalid server name");
    }

    servers_list.push_back(server);
}

void servers::remove_server_from_list(const std::string server) {
    if(auto it = std::find(servers_list.begin(), servers_list.end(), server); it != servers_list.end()) {
        servers_list.erase(it);
    }
}

std::vector<std::string> servers::get_list(void) const {
    return servers_list;
}

bool servers::validate_server_name(std::string_view server) const {
    // todo

    return true;
}

std::vector<server_ui> to_ui(std::vector<std::string> sl) {
    std::vector<server_ui> s_ui;

    for(const auto& s: sl) {
        s_ui.push_back({s, false});
    } 

    return s_ui;
}

std::vector<std::string> from_ui(std::vector<server_ui> s_ui) {
    std::vector<std::string> sl;

    for(const auto& s: s_ui) {
        sl.push_back(s.first);
    }

    return sl;
}