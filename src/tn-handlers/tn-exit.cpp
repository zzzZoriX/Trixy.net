#include <tn-handlers/tn-exit.hpp>
#include <tn-ui/tn-screen.hpp>
#include <cstdlib>


void handlers::tn_exit(ftxui::ScreenInteractive& screen) {
    auto exit_lambda = screen.ExitLoopClosure();
    
    exit_lambda();

    // todo: ui::reset_screen();
    std::system("clear");
}