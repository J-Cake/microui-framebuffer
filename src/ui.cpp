#include "include/ui.h"
#include <stdlib.h>

extern "C" {
#include "microui.h"
}

void UI(mu_Context *ctx) {
    const auto window_title = "Window Management Library";

    if (mu_begin_window(ctx, window_title, mu_rect(10, 10, 720, 480))) {
        if (mu_button(ctx, "Terminate"))
            exit(0);

        mu_end_window(ctx);
    }
}
