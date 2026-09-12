#include <iostream>
#include "core.hpp"
#include <boost/asio.hpp>


int main(void) {
    try {
        boost::asio::io_context ioc;

        auto core{std::make_shared<tn_core>("./examples/config.cfg", ioc)};

        core->run();

        ioc.run();

        core->stop();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;

        return 1;
    }

    return 0;
}