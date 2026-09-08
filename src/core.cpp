#include "core.hpp"


void tn_core::run(std::string_view cfgfp) {
    tn_cfg = handlers::config(cfgfp);

    tn_slm = handlers::servers(tn_cfg.get_servers_file());
    tn_ui = ui::UI(std::make_shared<tn_core>(this));
}

const handlers::servers* const tn_core::get_servers_list_service() {
    return &tn_slm;
}

const ui::UI* const tn_core::get_ui_service() {
    return &tn_ui;
}

const handlers::config* const tn_core::get_config_service() {
    return &tn_cfg;
}