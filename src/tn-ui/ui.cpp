#include <tn-ui/ui.hpp>
#include <tn-ui/tn-screen.hpp>
#include <tn-handlers/handlers.hpp>
#include <tn-network/tn-ping.hpp>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>

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

    std::vector<std::string> slist;
    
    if(const auto core_ptr = core_wptr.lock()) {
        slist = core_ptr->get_servers_list_service()->get_list();

        pings_list.reserve(slist.size());
        std::fill(pings_list.begin(), pings_list.end(), "0 ms");

        for(int server = 0; server < slist.size(); ++server) {
            std::string server_name = slist.at(server);

            auto row_component = Renderer([server_name, this, server] {
                return hbox({
                    text(server_name) | flex,
                    separator(),
                    text(pings_list.at(server))
                });
            });

            servers_list.push_back(row_component);
        }
    }

    auto ping_tab{Container::Vertical({
        Container::Vertical(servers_list),
        Button("Ping", [this, slist] {
            if(const auto core_ptr = core_wptr.lock()){
                core_ptr->get_ping_manager()->start(
                    slist,
                    [this, slist](network::ping_result result) {
                        if(auto server{std::find(slist.begin(), slist.end(), result.host)}; server != slist.end()) {
                            auto index = std::distance(slist.begin(), server);

                            pings_list.at(index) = std::to_string(result.avg_ping.count());
                        }
                    }
                );
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