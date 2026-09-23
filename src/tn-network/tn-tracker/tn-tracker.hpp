#pragma once

#include <string>
#include <functional>
#include <memory>
#include <pcapplusplus/PcapLiveDevice.h>
#include <tn-error_handling/loger.hpp>
#include <pcapplusplus/Packet.h>


namespace network {

using tracker_callback = std::function<void(std::string)>;

struct tracker_settings {
    bool    all_ports,
            IPv6,
            TCP,
            UDP,
            ETH;
    std::string device_name;
};


class tracker: public std::enable_shared_from_this<tracker> {
    tracker_settings settings;
    pcpp::PcapLiveDevice* device;
    std::shared_ptr<error_handling::loger> loger;
    
    tracker_callback callback;
    std::weak_ptr<tracker>* capture_cookie;

    std::atomic<int> packets_counter;

public:
    tracker(std::shared_ptr<error_handling::loger> loger)
    :   settings()
    ,   device(nullptr)
    ,   loger(loger)
    ,   capture_cookie(nullptr)
    ,   packets_counter(0) {}

    ~tracker() {
        stop();
    }


    void start(const tracker_settings& settings, tracker_callback callback);
    void stop();

private:
    void set_filters() const;

    static void on_packet_arrives(pcpp::RawPacket* rpack, pcpp::PcapLiveDevice* device, void* user_cookie);
    void handle_packet(pcpp::RawPacket* rpack);
};

}