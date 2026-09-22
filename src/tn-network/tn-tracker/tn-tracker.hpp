#pragma once

#include <string>
#include <functional>
#include <memory>
#include <pcapplusplus/PcapLiveDevice.h>
#include <tn-error_handling/loger.hpp>
#include <pcapplusplus/Packet.h>


namespace network {

using tracker_callback = std::function<void()>;

struct tracker_settings {
    bool    all_ports,
            IPv6,
            TCP,
            UDP,
            ETH;
    std::string device_name;
};


class tracker {
    tracker_settings settings;
    pcpp::PcapLiveDevice* device;
    tracker_callback callback;
    std::shared_ptr<error_handling::loger> loger;

public:
    tracker(std::shared_ptr<error_handling::loger> loger);

    void start(const tracker_settings& settings, tracker_callback callback);
    void stop();

private:
    void set_filters() const;
};

void on_packet_arrives(pcpp::RawPacket* rpack, pcpp::PcapLiveDevice* device, void* user_cookie);

}