/**
 * @brief This header is needed for the UI and Core binder class.
 */  

#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <tn-handlers/handlers.hpp>
#include <tn-ui/ui.hpp>


// As long as Trixy is not asynchronous, there is no need to use a mutex or `boost::asio::strand`.
class tn_core: public std::enable_shared_from_this<tn_core> {
    handlers::config tn_cfg;    // trixy-net config

    ui::UI tn_ui;               // trixy-net user interface
    
    handlers::servers tn_slm;   // trixy-net servers list manager
    // network services...
    // settings

public:
    tn_core() = default;

    void run(std::string_view cfgfp);

    void restart();

    void stop();


    const handlers::servers* const get_servers_list_service();
    const ui::UI* const get_ui_service();
    const handlers::config* const get_config_service();
};