#include "core.hpp"


void tn_core::run(std::string_view cfgfp, boost::asio::io_context& ioc) {
    tn_cfg = handlers::config(cfgfp);

    tn_slm = handlers::servers(tn_cfg.get_servers_file());
    tn_ui = ui::UI(shared_from_this());
    tn_loger = error_handling::loger(tn_cfg.get_logs_path());
    tn_pm = network::ping_manager(ioc, shared_from_this());
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

network::ping_manager* const tn_core::get_ping_manager() {
    return &tn_pm;
}