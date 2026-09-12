#include "core.hpp"


void tn_core::run(std::string_view cfgfp) {
    tn_cfg = handlers::config(cfgfp);

    tn_slm = handlers::servers(tn_cfg.get_servers_file());
    tn_ui = ui::UI(std::make_shared<tn_core>(this));
    tn_loger = error_handling::loger(tn_cfg.get_logs_path());
}

void tn_core::stop() {
    tn_loger.create_log();
}

handlers::servers* const tn_core::get_servers_list_service() {
    return &tn_slm;
}

ui::UI* const tn_core::get_ui_service() {
    return &tn_ui;
}

handlers::config* const tn_core::get_config_service() {
    return &tn_cfg;
}

error_handling::loger* const tn_core::get_loger() {
    return &tn_loger;
}