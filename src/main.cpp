#include <chrono>
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <libinput.h>
#include <linux/fb.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cairo.h>
#include <pango/pango-font.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>

extern "C" {
#include "microui.h"
}

#include "include/draw.h"
#include "include/io.h"
#include "include/ui.h"

fb *open_fb() {
    int fd = open("/dev/fb0", O_RDWR);

    if (fd < 0) {
        std::cerr << "Unable to open device" << std::endl;
        return nullptr;
    }

    struct fb_var_screeninfo vscreen_info;
    int err = ioctl(fd, FBIOGET_VSCREENINFO, &vscreen_info);

    size_t size = 4 * vscreen_info.xres * vscreen_info.yres;
    void *buf = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    fb *buffer = new fb{vscreen_info.xres, vscreen_info.yres, buf, (unsigned int)size};

    return buffer;
}

int getElapsedTime(const std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::duration<long int, std::ratio<1, 1000000000>>> start) {
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count();
}

int main() {
    auto dev = open_fb();

    const auto start = std::chrono::system_clock::now();

    init_draw(dev);

    mu_Context *ctx = (mu_Context *)malloc(sizeof(mu_Context));
    mu_init(ctx);

    init_event(dev, ctx);

    ctx->text_width = text_width;
    ctx->text_height = text_height;

    while (true) { // has it been 10s since the program started?
        mu_begin(ctx);

        cairo_set_source_rgba(cr, 1., 1., 1., 1.);
        cairo_rectangle(cr, 0, 0, dev->width, dev->height);
        cairo_fill(cr);

        UI(ctx);

        mu_end(ctx);

        flush(ctx, dev);
    }

    return 0;
}
