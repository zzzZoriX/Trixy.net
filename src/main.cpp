#include <iostream>
#include "core.hpp"
#include <boost/asio.hpp>
#include <thread>


int main(void) {
    try {
        boost::asio::io_context ioc;

        auto guard{boost::asio::make_work_guard(ioc)};

        std::thread net_thread([&] {
            ioc.run();
        });

        auto core{std::make_shared<tn_core>("./examples/config.cfg", ioc)};

        core->run();

        core->stop();
        guard.reset();
        ioc.stop();

        if(net_thread.joinable()){
            net_thread.join();
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;

        return 1;
    }

    return 0;
}