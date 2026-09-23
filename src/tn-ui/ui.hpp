#pragma once

#include <ftxui/ftxui.hpp>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <tn-network/tn-tracker/tn-tracker.hpp>

using namespace ftxui;

class tn_core;


namespace ui {

class UI {
    ScreenInteractive screen;
    Component renderer;

    Component actions_menu;
    Component action_container;
    Component servers_container;

    std::vector<std::string> actions_tab;
    std::vector<Component> servers_list;
    std::vector<std::string> pings_list;
    std::vector<std::string> packets_list;

    int action_selected,
        server_selected;
    network::tracker_settings tr_settings;

    std::weak_ptr<tn_core> core_wptr;

public:
    UI(std::shared_ptr<tn_core> core_wptr);

    /**
     * @brief Initialize the UI application.
     */
    void init_container();

    /**
     * @brief Run the UI application.
     */
    void run();

private:
    /**
     * @brief Initialize the UI components.
     */
    void init_components();
};

}