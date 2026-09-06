#include <tn-ui/tn-screen.hpp>
#include <thread>
#include <iostream>


void ui::new_screen() {
    std::cout << "\033[?1049h\033[H\033[2J" << std::flush; // Switch to the alternate screen buffer and move the cursor to the home position

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void ui::reset_screen() {
    std::cout << "\033[2J\033[?1049l" << std::flush; // Switch back to the normal screen buffer

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}