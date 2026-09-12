#include "tn-ping.hpp"
#include <iostream>
#include <format>
#include "../core.hpp"

using namespace network;

ping_manager::ping_manager(io_context& ioc, std::shared_ptr<tn_core> core)
:   ioc(ioc)
,   core(core) {}

void ping_manager::start(std::vector<std::string> hosts, ping_callback callback) const {
    for(const auto& host: hosts)
        std::make_shared<pinger>(host, ioc, callback, core)->start();
}


pinger::pinger(const std::string& host, io_context& ioc, ping_callback callback, std::weak_ptr<tn_core> core)
:   host(host)
,   resolver(ioc)
,   ioc(ioc)
,   sock(ioc, ip::icmp::v4())
,   timer(ioc)
,   seq_num(0)
,   timeout(5)
,   core(core) {
    dest = *resolver.resolve(ip::icmp::v4(), host, "").begin();
}

void pinger::start() {
    start_send();
    start_receive();
}

void pinger::start_send() {
    std::string body{"trixy-ping"};

    icmp_header echo_req;

    echo_req.type(icmp_header::echo_request);
    echo_req.code(0);
    echo_req.id(get_id());
    echo_req.seq_num(++seq_num);

    compute_check_sum(echo_req, body.begin(), body.end());

    streambuf req_buffer;

    std::ostream os{&req_buffer};
    os << echo_req << body;

    time_sent = std::chrono::steady_clock::now();

    sock.send_to(
        req_buffer.data(),
        dest
    );

    timer.expires_after(timeout);
    timer.async_wait([this, self = shared_from_this()] (system::error_code ec) {
        handle_timeout(ec);
    });
}

void pinger::handle_timeout(system::error_code ec) {
    system::error_code sock_ec;

    sock.cancel(sock_ec);
    sock.close(sock_ec);

    if(ec) {
        if(auto core_ptr = core.lock()) {
            core_ptr->get_loger()->add_log(
                std::format("Error in timeout: {}", ec.message()),
                TIMEOUT_ABORT
            );
        }

        callback(ERROR_PING(host));
    }
    else {
        callback(TIMEOUT_PING(host));
    }
}

void pinger::start_receive() {
    sock.async_receive(
        buffer.prepare(65536), // 64KB
        [this, self = shared_from_this()] (system::error_code ec, std::size_t len) {
            handle_receive(ec, len);
        }
    );
}

void pinger::handle_receive(system::error_code ec, std::size_t len) {
    if(ec) {
        if(auto core_ptr = core.lock()) {
            core_ptr->get_loger()->add_log(
                std::format("Error occured when reply was received: {}", ec.message()),
                READ_ABORT
            );
        }

        callback(ERROR_PING(host));
    }
    else {
        buffer.commit(len);

        std::istream is{&buffer};

        icmp_header icmp_head;
        ipv4_header ipv4_head;

        is >> ipv4_head >> icmp_head;

        if(
            is  &&
            icmp_head.type() == icmp_header::echo_reply   &&
            icmp_head.id() == get_id()  &&
            icmp_head.seq_num() == seq_num
        ) {
            auto now{std::chrono::steady_clock::now()};

            auto elapsed{
                std::chrono::duration_cast<ping_time>(now - time_sent)
            };

            callback(ping_result(elapsed, host));
        }
    }
}