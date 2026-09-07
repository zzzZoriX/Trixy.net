#include "core.hpp"


void tn_core::run(std::string_view cfgfp) {
    tn_cfg = handlers::config(cfgfp);

    tn_slm = handlers::servers(tn_cfg.get_servers_file());
}