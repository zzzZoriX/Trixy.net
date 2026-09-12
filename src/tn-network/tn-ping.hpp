#pragma once

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <string_view>
#include <memory>

#include "ipv4_header.hpp"
#include "icmp_header.hpp"
#include "../core.hpp"

using ping_time = std::chrono::milliseconds;
using namespace boost::asio;
using namespace boost;



namespace network {
    
struct ping_result {
    ping_time   avg_ping;
    std::string host;
        
    ping_result(
        const int avg,
        std::string_view host
    )
    :   host(host)
    ,   avg_ping(avg) {}

    ping_result()
    :   host("")
    ,   avg_ping(0) {}

    ping_result(
        const ping_time avg,
        std::string_view host
    )
    :   host(host)
    ,   avg_ping(avg) {}
};

using ping_callback = std::function<void(ping_result)>;
#define ERROR_PING(host) ping_result(35505, host)
#define TIMEOUT_PING(host) ping_result(7107111, host)
    
class pinger: public std::enable_shared_from_this<pinger> {
    io_context& ioc;
    std::string host;
    std::weak_ptr<tn_core> core;

    steady_timer timer;
    unsigned short seq_num;
    std::chrono::steady_clock::time_point time_sent;
    streambuf buffer;

    ip::icmp::resolver resolver;
    ip::icmp::endpoint dest;
    ip::icmp::socket sock;

    ping_callback callback;
    
    std::chrono::seconds timeout;

    ping_result result;

public:
    pinger(const std::string& host, io_context& ioc, ping_callback callback, std::weak_ptr<tn_core> core);

    void start();

private:
    void start_send();
    void handle_timeout(system::error_code ec);
    void start_receive();
    void handle_receive(system::error_code ec, std::size_t len);
};

class ping_manager {
    io_context& ioc;
    std::weak_ptr<tn_core> core;

public:
    ping_manager(io_context& ioc, std::shared_ptr<tn_core> core);

    ping_manager& operator=(const ping_manager& other); // idk why, but this operator overload function declare fix the problem in core.cpp at tn_pm init line in run function

    void start(std::vector<std::string> hosts, ping_callback callback) const;
};

};