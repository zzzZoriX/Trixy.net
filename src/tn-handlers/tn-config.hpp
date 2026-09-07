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

    std::string get_logs_path() const;
    void update_logs_path(const std::string&& logs_path);
    void update_servers_file(const std::string&& sfn);
    void update_settings_file(const std::string&& stfn);
};

}