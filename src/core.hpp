/**
 * @brief This header is needed for the UI and Core binder class.
 */  

#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <tn-handlers/handlers.hpp>


// As long as Trixy is not asynchronous, there is no need to use a mutex or `boost::asio::strand`.
class tn_core {
    handlers::config tn_cfg;    // trixy-net config
    
    handlers::servers tn_slm;   // trixy-net servers list manager
    // network services...
    // settings

public:
    tn_core() = default;

    void run(std::string_view cfgfp);

    void restart();

    void stop();
};