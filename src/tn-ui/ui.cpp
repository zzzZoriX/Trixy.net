#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <iterator>
#include <memory>
#include <tn-ui/ui.hpp>
#include <tn-ui/tn-screen.hpp>
#include <tn-servers/tn-servers.hpp>
#include <tn-config/tn-config.hpp>
#include <tn-network/tn-ping.hpp>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <format>
#include "../core.hpp"
#include "tn-themes/tn-themes.hpp"

using namespace ftxui;


ui::UI::UI(std::string_view user_theme, std::shared_ptr<tn_core> core_wptr)
:   screen{ScreenInteractive::TerminalOutput()}
,   core_wptr(core_wptr)
,   tr_settings(network::tracker_settings(false, false, false, false, false, false, false, false, false, "wlx503dd1ffd15f"))
,   user_theme() {
    this->user_theme.set_theme(user_theme);

    auto it{std::find(themes_names.begin(), themes_names.end(), theme::convert(this->user_theme))};

    if(it != themes_names.end())
        selected_theme = std::distance(themes_names.begin(), it);
    else 
        selected_theme = 0;
}

void ui::UI::load_core_ptr(std::shared_ptr<tn_core> core_ptr) {
    core_wptr.reset();
    core_wptr = core_ptr;
}

void ui::UI::init_container() {
    init_components();

    auto container{Container::Horizontal({actions_menu, action_container})};

    renderer = Renderer(container, [this] {
        return vbox({
            text("Trixy.net") | bold | center | color(Color::RGB(user_theme.paragraph[0], user_theme.paragraph[1], user_theme.paragraph[2])),
            separator() | color(Color::RGB(user_theme.borders[0], user_theme.borders[1], user_theme.borders[2])),
            hbox({
                vbox({
                    text("Features:") | bold | center | color(Color::RGB(user_theme.paragraph[0], user_theme.paragraph[1], user_theme.paragraph[2])),
                    actions_menu->Render() | bold | center | color(Color::RGB(user_theme.text[0], user_theme.text[1], user_theme.text[2]))
                }),
                separator() | color(Color::RGB(user_theme.borders[0], user_theme.borders[1], user_theme.borders[2])),
                action_container->Render()
            })
        }) | borderStyled(Color::RGB(user_theme.borders[0], user_theme.borders[1], user_theme.borders[2]));
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
        "Themes",
        "Trixy"
    };
    action_selected = 0;
    actions_menu = Menu(&actions_tab, &action_selected);

    ButtonOption bopt;
    bopt.transform = [this](const EntryState& s) {
        auto element{text(s.label) | center};

        element |= color(Color::RGB(user_theme.buttons_text[0], user_theme.buttons_text[1], user_theme.buttons_text[2]));
        if(s.focused) element |= bold;

        element = border(element);

        element |= color(Color::RGB(user_theme.borders[0], user_theme.borders[1], user_theme.borders[2]));

        return element;
    };

    RadioboxOption ropt;
    ropt.on_change = [this]() {
           
    };

    std::vector<std::string> slist;
    
    if(const auto core_ptr = core_wptr.lock()) {
        slist = core_ptr->get_servers_list_service()->get_list();

        pings_list.resize(slist.size(), "0 ms");

        for(int server = 0; server < slist.size(); ++server) {
            std::string server_name = slist.at(server);

            auto row_component = Renderer([server_name, this, server] {
                return hbox({
                    text(server_name) | flex | color(Color::RGB(user_theme.text[0], user_theme.text[1], user_theme.text[2])),
                    separator() | color(Color::RGB(user_theme.borders[0], user_theme.borders[1], user_theme.borders[2])),
                    text(pings_list.at(server)) | color(Color::RGB(user_theme.text[0], user_theme.text[1], user_theme.text[2]))
                });
            });

            servers_list.push_back(row_component);
        }
    }

    if (!servers_list.empty()) {
        servers_container = Container::Vertical(servers_list);
    } 
    else {
        servers_container = Renderer([this] { return text("No servers loaded") | dim | color(Color::RGB(user_theme.text[0], user_theme.text[1], user_theme.text[2])); });
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
        }, bopt)
    })};

    auto traffic_terminal{Renderer([this] {
        Elements elements;

        size_t total_packets = packets_list.size();

        for(size_t i = 0; i < total_packets; ++i) {
            if(i == total_packets - 1) {
                elements.push_back(text(packets_list[i]) | focus | color(Color::RGB(user_theme.text[0], user_theme.text[1], user_theme.text[2])));
            } 
            else {
                elements.push_back(text(packets_list[i]) | color(Color::RGB(user_theme.text[0], user_theme.text[1], user_theme.text[2])));
            }
        }

        return vbox(std::move(elements)) | vscroll_indicator | yframe | yflex | focusPositionRelative(0, 1);
    })};

    auto traffic_terminal_window{Renderer(traffic_terminal, [traffic_terminal, this] {
        return window(
            text("Traffic terminal") | bold | center | color(Color::RGB(user_theme.paragraph[0], user_theme.paragraph[1], user_theme.paragraph[2])),
            traffic_terminal->Render() | flex
        ) | color(Color::RGB(user_theme.borders[0], user_theme.borders[1], user_theme.borders[2]));
    })};

    CheckboxOption option;
    option.transform = [this](const EntryState& state) {
        auto t = text((state.state ? "[x] " : "[ ] ") + state.label);
        if (state.focused) {
            return t | color(Color::RGB(
                user_theme.selected[0],
                user_theme.selected[1],
                user_theme.selected[2]
            )) | bold;
        }

        return t | color(Color::RGB(
                user_theme.buttons_text[0],
                user_theme.buttons_text[1],
                user_theme.buttons_text[2]
            ));
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

    auto settings_window{Renderer(settings, [settings, this] {
        return window(
            text("Tracker settings") | bold | center | color(Color::RGB(user_theme.paragraph[0], user_theme.paragraph[1], user_theme.paragraph[2])),
            settings->Render()
        ) | color(Color::RGB(user_theme.borders[0], user_theme.borders[1], user_theme.borders[2]));
    })};

    auto traffic_buttons{Container::Horizontal({
        Button("Start tracking", [this] {
            if(const auto core_ptr = core_wptr.lock()) {
                core_ptr->get_tracker()->start(tr_settings, [this](std::string pi) {
                    packets_list.push_back(pi);

                    screen.PostEvent(Event::Custom);
                });

            }
        }, bopt),
        Button("Stop tracking", [this] {
            if(const auto core_ptr = core_wptr.lock()) {
                core_ptr->get_tracker()->stop();
            }
        }, bopt)
    })};

    auto traffic_tab{Container::Horizontal({
        Container::Vertical({
            traffic_terminal_window,
            traffic_buttons
        }) | flex,
        settings_window
    })};

    auto theme_radioboxes{Radiobox({
        themes_names
    }, &selected_theme)};  

    theme_selector = CatchEvent(theme_radioboxes, [theme_radioboxes, this](Event e) {
        if(!theme_radioboxes->OnEvent(e)) return false;

        user_theme.set_theme(themes_names[selected_theme]);

        screen.PostEvent(Event::Custom);

        return true;
    });

    auto themes_window {Renderer(theme_selector, [this] {
        return window(
            text("Themes") | bold | center | color(Color::RGB(user_theme.paragraph[0], user_theme.paragraph[1], user_theme.paragraph[2])),
            theme_selector->Render()
        ) | color(Color::RGB(user_theme.borders[0], user_theme.borders[1], user_theme.borders[2]));
    })};

    auto trixy_tab{Container::Vertical({
        Container::Horizontal({
            Button("Exit", [this] { 
                if(const auto core_ptr = core_wptr.lock()) {
                    screen.Exit();
                }          
            }, bopt),
            Button("Restart", [] {}, bopt)
        })
    })};

    action_container = Container::Tab(
        {
            ping_tab,
            traffic_tab,
            themes_window,
            trixy_tab
        },
        &action_selected
    );
}

theme ui::UI::get_theme(void) const {
    return user_theme;
}
