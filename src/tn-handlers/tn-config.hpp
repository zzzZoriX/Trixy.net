#pragma once

#include <string>
#include <string_view>
#include <vector>


namespace handlers {

class config {
    static std::string config_file_path;
    static struct {
        std::string settings_file,
                    servers_file,
                    logs_path;
    } files;

public:
    config() = delete;

    static void load_config(std::string_view config_file_path);
    static void update_config();

    static void get_settings(); // todo: replace return void type with a struct containing the settings data
    static std::vector<std::string> get_servers_list();
    static std::string get_logs_path();

    static void update_settings(void); // todo: void param type should be replaced with a struct containing the settings data

    static void update_servers_list(const std::vector<std::string>& servers_list);
    static void add_server_to_list(const std::string& server);
    static void remove_server_from_list(const std::string& server);
    
    static void update_logs_path(const std::string& logs_path);
};

}