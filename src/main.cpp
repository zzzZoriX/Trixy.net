#include <iostream>
#include "core.hpp"


int main(void) {
    auto core{std::make_shared<tn_core>()};

    core->run("../examples/config.cfg");

    return 0;
}