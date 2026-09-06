#include <iostream>
#include <tn-ui/ui.hpp>
#include <binder.hpp>


int main(void) {
    auto const tn_binder{std::make_shared<binder>()};

    ui::UI ui_manager{tn_binder};

    ui_manager.init_container();
    ui_manager.run();

    return 0;
}