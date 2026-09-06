#include <tn-ui/ui.hpp>
#include <tn-ui/tn-screen.hpp>
#include <tn-handlers/handlers.hpp>
#include <cstdlib>
#include <vector>
#include <string>

using namespace ftxui;


ui::UI::UI()
:   screen{ScreenInteractive::TerminalOutput()} {}

void ui::UI::init_container() {
    init_components();

    auto container{Container::Vertical({features_container})};

    renderer = Renderer(container, [this] {
        return vbox({
            text("Trixy.net") | bold | center | color(Color::RGB(0, 179, 255)),
            separator(),
            hbox({
                vbox({
                    text("Features:") | bold | center | color(Color::RGB(0, 255, 171)),
                    features_container->Render() | bold | center |color(Color::RGB(0, 164, 109))
                }),
                separator(),
                vbox({
                    text("Result:") | bold | center | color(Color::RGB(255, 255, 0)),
                })
            })
        }) | border;
    });
}

void ui::UI::run() {
    // todo: ui::new_screen();
    std::system("clear");
    
    screen.Loop(renderer);
}

void ui::UI::init_components() {
    Component button_ps{Button("Ping server", [] {})};
    Component button_psl{Button("Ping servers list", [] {})};
    Component button_st{Button("Show traffic", [] {})};
    Component button_uc{Button("Update config", [] {})};
    Component button_s{Button("Settings", [] {})};
    Component button_e{Button("Exit", [this] { 
        handlers::tn_exit(screen); 
    })};

    features_container = Container::Vertical({
        button_ps,
        button_psl,
        button_st,
        button_uc,
        button_s,
        button_e
    });
}