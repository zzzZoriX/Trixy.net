#pragma once

#include <ftxui/ftxui.hpp>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <binder.hpp>

using namespace ftxui;


namespace ui {

class UI {
    std::shared_ptr<binder> tn_binder;

    ScreenInteractive screen;
    Component renderer;

    Component actions_menu;
    Component action_container;

    std::vector<std::string> actions_tab;
    int action_selected;

    std::map<std::string, std::map<std::string, Component>> actions_map;

public:
    UI(std::shared_ptr<binder> binder);

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