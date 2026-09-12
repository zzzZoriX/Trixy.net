#include <iostream>
#include "core.hpp"
#include <boost/asio.hpp>


int main(void) {
    try {
        boost::asio::io_context ioc;

        auto core{std::make_shared<tn_core>()};

        core->run("../examples/config.cfg", ioc);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;

        return 1;
    }

    return 0;
}