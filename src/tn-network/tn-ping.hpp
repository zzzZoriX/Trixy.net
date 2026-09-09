#pragma once

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <string_view>

#include "ipv4_header.hpp"
#include "icmp_header.hpp"

using ping_time = std::chrono::milliseconds;
using namespace boost::asio;

using ping_callback = std::function<void()>;


namespace network {

struct ping_result {
    ping_time   avg_ping,
                max_ping,
                min_ping;
    std::string host;

    ping_result(
        const ping_time avg,
        const ping_time max,
        const ping_time min,
        std::string_view host
    )
    :   host(host)
    ,   avg_ping(avg)
    ,   max_ping(max)
    ,   min_ping(min) {}
};

class ping {
    io_context& ioc;
    std::string host;

    steady_timer timer;
    unsigned short seq_num;
    chrono::steady_clock::time_point time_sent;
    streambuf buffer;

    ip::icmp::resolver resolver;
    ip::icmp::endpoint dest;
    ip::icmp::socket sock;

    ping_callback callback;

public:
    ping(const std::string& host, io_context& ioc, ping_callback callback);

private:
    void start_send();
    void handle_timeout();
    void start_receive();
    void hadnle_receive();
};

class pinger {
    io_context& ioc;

public:
    pinger(io_context& ioc);

    std::vector<ping_result> start(std::vector<std::string> hosts, ping_callback callback) const;
};

};