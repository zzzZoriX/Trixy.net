#pragma once

#include <vector>
#include <string>
#include <string_view>


namespace handlers {

class servers {
    std::vector<std::string> servers_list;
    std::string slist_fn;   // variable that store the name of servers list file

public:
    servers(std::string_view slist_fn);
    ~servers();

    void add_server_to_list(const std::string server);
    void remove_server_from_list(const std::string server);
    std::vector<std::string> get_list(void) const;

private:
    bool validate_server_name(const std::string_view server) const;
};
    
}