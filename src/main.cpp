#include <iostream>
#include <tn-ui/ui.hpp>


int main(void) {
    ui::UI ui_manager;

    ui_manager.init_container();
    ui_manager.run();

    return 0;
}