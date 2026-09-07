#pragma once

#include <string>
#include <string_view>
#include <vector>


namespace handlers {

class config {
    std::string config_file_path;
    struct {
        std::string settings_file,
                    servers_file,
                    logs_path;
    } files;

public:
    config(std::string_view config_file_path);
    ~config();

    void get_settings() const; // todo: replace return void type with a struct containing the settings data
    std::vector<std::string> get_servers_list() const;
    std::string get_logs_path() const;

    void update_settings(void) const; // todo: void param type should be replaced with a struct containing the settings data
    void add_server_to_list(const std::string& server) const;
    void remove_server_from_list(const std::string& server) const;
    void update_logs_path(const std::string& logs_path);
};

}