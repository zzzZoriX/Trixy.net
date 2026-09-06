#include <binder.hpp>

std::string* binder::get_host_to_ping_as_ptr(void) {
    return &host_to_ping;
}

std::string binder::get_host_to_ping(void) const {
    return host_to_ping;
}