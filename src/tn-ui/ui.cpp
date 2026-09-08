#include <tn-ui/ui.hpp>
#include <tn-ui/tn-screen.hpp>
#include <tn-handlers/handlers.hpp>
#include <cstdlib>
#include <vector>
#include <string>

using namespace ftxui;


ui::UI::UI(std::shared_ptr<tn_core> core_wptr)
:   screen{ScreenInteractive::TerminalOutput()}
,   core_wptr(core_wptr) {}

void ui::UI::init_container() {
    init_components();

    auto container{Container::Horizontal({actions_menu, action_container})};

    renderer = Renderer(container, [this] {
        return vbox({
            text("Trixy.net") | bold | center | color(Color::RGB(0, 179, 255)),
            separator(),
            hbox({
                vbox({
                    text("Features:") | bold | center | color(Color::RGB(0, 255, 171)),
                    actions_menu->Render() | bold | center | color(Color::RGB(0, 164, 109))
                }),
                separator(),
                action_container->Render()
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
    actions_tab = {
        "Ping",
        "Show traffic",
        "Settings",
        "Trixy"
    };
    action_selected = 0;
    actions_menu = Menu(&actions_tab, &action_selected);

    if(const auto core_ptr = core_wptr.lock())
        s_ui_list = handlers::to_ui(core_ptr->get_servers_list_service()->get_list());

    for(auto& s: s_ui_list) {
        servers_checkboxes.push_back(Checkbox(
            s.first, &s.second
        ));
    }

    auto ping_tab{Container::Vertical({
        servers_checkboxes,
        Button("Ping", [this] {
            if(const auto core_ptr = core_wptr.lock()){
                // todo
            }
        })
    })};

    auto traffic_tab{Container::Vertical({
        Button("Start tracking", [] {})
    })};

    auto settings_tab{Container::Vertical({
        
    })};

    auto trixy_tab{Container::Vertical({
        Container::Horizontal({
            Button("Exit", [this] { handlers::tn_exit(screen); }),
            Button("Restart", [] {})
        })
    })};

    action_container = Container::Tab(
        {
            ping_tab,
            traffic_tab,
            settings_tab,
            trixy_tab
        },
        &action_selected
    );
}