#pragma once

#include <ftxui/ftxui.hpp>

namespace ui {

class UI {
    ftxui::ScreenInteractive screen;
    ftxui::Component renderer;

    ftxui::Component features_container;

public:
    UI();

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