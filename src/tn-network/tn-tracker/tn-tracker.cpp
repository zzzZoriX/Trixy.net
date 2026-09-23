#include <format>
#include <pcapplusplus/PcapLiveDeviceList.h>
#include <pcapplusplus/EthLayer.h>
#include <pcapplusplus/IPv4Layer.h>
#include <pcapplusplus/TcpLayer.h>
#include <pcapplusplus/UdpLayer.h>
#include <pcapplusplus/SystemUtils.h>
#include "tn-tracker.hpp"
#include <sstream>

using namespace network;


void tracker::start(const tracker_settings& settings, tracker_callback callback) {
    this->settings = settings;
    this->callback = std::move(callback);

    const auto& dev_list{pcpp::PcapLiveDeviceList::getInstance()};

    device = dev_list.getDeviceByName(settings.device_name);

    if(!device) {
        loger->add_log(std::format("Device not found {}", settings.device_name), SOMETHING_NOT_FOUND);

        return;
    }

    if(!device->open()) {
        loger->add_log(std::format("Can't open device {}", settings.device_name), CANT_OPEN_SOMETHING);

        return;
    }

    loger->add_log(std::format("Device successfully open {}", settings.device_name), OPEN_SUCCESS);

    set_filters();

    loger->add_log(std::format("Set filtres for device {}", settings.device_name), SET_SUCCESS);

    capture_cookie = new std::shared_ptr<tracker>(shared_from_this());

    if(!device->startCapture(on_packet_arrives, capture_cookie)) {
        delete capture_cookie;

        loger->add_log("Can't start tracking", CANT_START);

        return;
    }

    loger->add_log("Tracking started successfully", START_SUCCESS);
}

void tracker::stop() {
    delete capture_cookie;
    
    if(device && device->captureActive()) {
        device->stopCapture();
        device->close();

        loger->add_log("Tracking ended succesfully", END_SUCCESS);
    }
    else {
        loger->add_log("Can't end tracking", CANT_END);
    }
}

void tracker::set_filters() const {
    pcpp::AndFilter result_filter;

    pcpp::ProtoFilter   ipv4{pcpp::IPv4},
                        ipv6{pcpp::IPv6};

    pcpp::OrFilter ip_filter;

    if(settings.IPv6) {
        ip_filter.addFilter(&ipv4);
        ip_filter.addFilter(&ipv6);

        result_filter.addFilter(&ip_filter);
    }
    else {
        result_filter.addFilter(&ipv4);
    }

    if(!settings.all_ports) {
        pcpp::PortFilter    port80{80, pcpp::SRC_OR_DST},
                            port443{443, pcpp::SRC_OR_DST};

        pcpp::OrFilter port_filter;

        port_filter.addFilter(&port80);
        port_filter.addFilter(&port443);

        result_filter.addFilter(&port_filter);
    }

    device->setFilter(result_filter);
}

void tracker::on_packet_arrives(pcpp::RawPacket* rpack, pcpp::PcapLiveDevice* device, void* user_cookie) {
    if(!user_cookie) {
        return;
    }

    auto* self_ptr{static_cast<std::shared_ptr<tracker>*>(user_cookie)};
    auto self{*self_ptr};

    if(self) {
        self->handle_packet(rpack);
    }
}

void tracker::handle_packet(pcpp::RawPacket* rpack) {
    pcpp::Packet pack{rpack};
    std::stringstream result;

    result << std::format("\n[Packet #{} info]\n", ++packets_counter);

    if(auto* eth_layer{pack.getLayerOfType<pcpp::EthLayer>()}; eth_layer != nullptr) {
        result << std::format("|- MAC: (src){} -> (dst){}\n", eth_layer->getSourceMac().toString(), eth_layer->getDestMac().toString());
    }

    if(auto* ip_layer{pack.getLayerOfType<pcpp::EthLayer>()}; ip_layer != nullptr) {
        result << std::format("|- IP: (src){} -> (dst){}\n", ip_layer->getSourceMac().toString(), ip_layer->getDestMac().toString());

        if(auto* tcp_layer{pack.getLayerOfType<pcpp::TcpLayer>()}; tcp_layer != nullptr) {
            result << std::format("|- TCP ports: (src){} -> (dest){}\n", tcp_layer->getSrcPort(), tcp_layer->getDstPort());

            result << std::format("|- TCP syn flag: {}", (tcp_layer->getTcpHeader()->synFlag == 1 ? "true" : "false"));
        }
        if(auto* udp_layer{pack.getLayerOfType<pcpp::UdpLayer>()}; udp_layer != nullptr) {
            result << std::format("|- UDP ports: (src){} -> (dest){}\n", udp_layer->getSrcPort(), udp_layer->getDstPort());
        }
    }

    result << "[Packet end]";

    loger->add_log(result.str(), SUCCESS);

    callback(result.str());
}