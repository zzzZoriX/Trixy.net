#include "tn-servers.hpp"
#include <fstream>
#include <array>
#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <filesystem>
#include <format>
#include "../core.hpp"
#include <tn-error_handling/error_codes.hpp>

using namespace handlers;


servers::servers(std::string_view slist_fn, std::shared_ptr<tn_core> core_)
:   slist_fn(slist_fn)
,   core(core_) {
    std::filesystem::path file_path{slist_fn};
    std::ifstream slist_fp{file_path};

    if (!slist_fp.is_open()) {
        throw std::runtime_error("Failed to open servers list file: " + file_path.string());
    }

    std::string line;
    while (std::getline(slist_fp, line)) {
        boost::algorithm::trim(line);

        if (line.empty()) continue;

        servers_list.push_back(line);

        if(auto core_ptr = core.lock()) {
            core_ptr->get_loger()->add_log(
                std::format("Loaded server - {}", line),
                SUCCESS
            );
        }
    }

    slist_fp.close();
}

void servers::commit() const {
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