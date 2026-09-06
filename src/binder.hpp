/**
 * @brief This header is needed for the UI and Core binder class.
 */  

#pragma once

#include <string>
#include <memory>


// As long as Trixy is not asynchronous, there is no need to use a mutex or `boost::asio::strand`.
class binder {
    // ping
    std::string host_to_ping;

    // traffic

    // settings

    // trixy

public:
    std::string* get_host_to_ping_as_ptr(void);
    std::string get_host_to_ping(void) const;
};