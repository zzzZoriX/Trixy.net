#include "core.hpp"


void tn_core::run(std::string_view cfgfp) {
    tn_cfg = handlers::config(cfgfp);

    
}