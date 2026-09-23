#include <tn-ui/ui.hpp>
#include <tn-ui/tn-screen.hpp>
#include <tn-handlers/handlers.hpp>
#include <tn-network/tn-ping.hpp>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <format>
#include "../core.hpp"

using namespace ftxui;


ui::UI::UI(std::shared_ptr<tn_core> core_wptr)
:   screen{ScreenInteractive::TerminalOutput()}
,   core_wptr(core_wptr)
,   tr_settings(network::tracker_settings(false, false, false, false, false, false, false, false, false, "wlx503dd1ffd15f")) {}

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

        pings_list.resize(slist.size(), "0 ms");

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

    if (!servers_list.empty()) {
        servers_container = Container::Vertical(servers_list);
    } 
    else {
        servers_container = Renderer([] { return text("No servers loaded") | dim; });
    }

    auto ping_tab{Container::Vertical({
        servers_container,
        Button("Ping", [this, slist] {
            try {
                if(const auto core_ptr = core_wptr.lock()){
                    core_ptr->get_ping_manager()->start(
                        slist,
                        [this, slist](network::ping_result result) {
                            screen.Post([this, slist, result] {
                                if(auto server = std::find(slist.begin(), slist.end(), result.host); server != slist.end()) {
                                    auto index{std::distance(slist.begin(), server)};

                                    if(index < pings_list.size()) {
                                        pings_list.at(index) = std::to_string(result.avg_ping.count()) + " ms";

                                        if(const auto core_ptr = core_wptr.lock()) {
                                            core_ptr->get_loger()->add_log(
                                                std::format("{} ping getted: {}", result.host, result.avg_ping),
                                                PING_SUCCESS
                                            );
                                        }
                                    }
                                    else {
                                        if(const auto core_ptr = core_wptr.lock()) {
                                            core_ptr->get_loger()->add_log(
                                                std::format("Unknown server: {}", *server),
                                                PING_ERROR
                                            );
                                        }
                                    }
                                }
                            });
                        }
                    );
                }
            }
            catch (const std::exception& e) {
                if(const auto core_ptr = core_wptr.lock()) {
                    core_ptr->get_loger()->add_log(
                        std::format("Ping exception: {}", e.what()),
                        PING_ERROR
                    );
                }
            }
        })
    })};

    auto traffic_terminal{Renderer([this] {
        Elements elements;

        size_t total_packets = packets_list.size();

        for(size_t i = 0; i < total_packets; ++i) {
            if(i == total_packets - 1) {
                elements.push_back(text(packets_list[i]) | focus);
            } 
            else {
                elements.push_back(text(packets_list[i]));
            }
        }

        return vbox(std::move(elements)) | vscroll_indicator | yframe | yflex | focusPositionRelative(0, 1);
    })};

    auto traffic_terminal_window{Renderer(traffic_terminal, [traffic_terminal] {
        return window(
            text("Traffic terminal") | bold | center | color(Color::RGB(0, 179, 255)),
            traffic_terminal->Render() | flex
        );
    })};

    CheckboxOption option;
    option.transform = [](const EntryState& state) {
        auto t = text((state.state ? "[x] " : "[ ] ") + state.label);
        if(state.focused) {
            return t | color(Color::Cyan) | bold;
        }
        return t;
    };

    auto settings{Container::Vertical({
        Checkbox("Log packets", &tr_settings.log_packets, option),
        Checkbox("Use all ports", &tr_settings.all_ports, option),
        Checkbox("Use IPv6", &tr_settings.IPv6, option),
        Checkbox("Check ETH", &tr_settings.ETH, option),
        Checkbox("Check TCP", &tr_settings.TCP, option),
        Checkbox("Check UDP", &tr_settings.UDP, option),
        Checkbox("Check DNS", &tr_settings.DNS, option),
        Checkbox("Show TLS", &tr_settings.TLS, option),
        Checkbox("Show HTTP", &tr_settings.HTTP, option)
    })};

    auto settings_window{Renderer(settings, [settings] {
        return window(
            text("Tracker settings") | bold | center | color(Color::RGB(0, 179, 255)),
            settings->Render()
        );
    })};

    auto traffic_buttons{Container::Horizontal({
        Button("Start tracking", [this] {
            if(const auto core_ptr = core_wptr.lock()) {
                core_ptr->get_tracker()->start(tr_settings, [this](std::string pi) {
                    packets_list.push_back(pi);

                    screen.PostEvent(Event::Custom);
                });

            }
        }),
        Button("Stop tracking", [this] {
            if(const auto core_ptr = core_wptr.lock()) {
                core_ptr->get_tracker()->stop();
            }
        })
    })};

    auto traffic_tab{Container::Horizontal({
        Container::Vertical({
            traffic_terminal_window,
            traffic_buttons
        }) | flex,
        settings_window
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