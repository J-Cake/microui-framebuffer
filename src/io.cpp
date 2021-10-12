#include <algorithm>
#include <array>
#include <fcntl.h>
#include <iostream>
#include <libinput.h>
#include <libudev.h>
#include <linux/input-event-codes.h>
#include <poll.h>
#include <string>
#include <thread>
#include <unistd.h>

#include "include/draw.h"
#include "include/io.h"
#include "microui.h"

const static struct libinput_interface interface = {.open_restricted = [](const char *path, int flags, void *user_data) -> int {
                                                        int fd = open(path, flags);
                                                        return fd < 0 ? -errno : fd;
                                                    },
                                                    .close_restricted = [](int fd, void *user_data) { close(fd); }};

void init_event(fb *dev, mu_Context *ctx) {
    udev *udev_ctx = udev_new();
    libinput *li = libinput_udev_create_context(&interface, NULL, udev_ctx);
    int fd = -1;

    if (libinput_udev_assign_seat(li, "seat0") != 0)
        std::cerr << "An error occurred initialising libinput\n";

    else if ((fd = libinput_get_fd(li)) < 1)
        std::cerr << "An error occurred opening filedescriptor\n";

    std::thread([=]() {
        while (true)
            poll_events(dev, ctx, li, fd);
    }).detach();
}

void handlers::move(mu_Context *ctx, libinput_event *event, fb *dev) {
    auto pointer = libinput_event_get_pointer_event(event);

    int x = std::clamp((int)libinput_event_pointer_get_dx(pointer) + ctx->mouse_pos.x, 0, (int)dev->width);
    int y = std::clamp((int)libinput_event_pointer_get_dy(pointer) + ctx->mouse_pos.y, 0, (int)dev->height);

    mu_input_mousemove(ctx, x, y);
}

void handlers::mousebtn(mu_Context *ctx, libinput_event *event, fb *dev) {
    auto pointer = libinput_event_get_pointer_event(event);

    uint32_t button_id = libinput_event_pointer_get_button(pointer);
    bool isDown = libinput_event_pointer_get_button_state(pointer) == LIBINPUT_BUTTON_STATE_PRESSED;

    int button = 0;

    if (button_id == BTN_LEFT)
        button = 1;
    else if (button_id == BTN_MIDDLE)
        button = 2;
    else if (button_id == BTN_RIGHT)
        button = 3;

    if (isDown)
        mu_input_mousedown(ctx, ctx->mouse_pos.x, ctx->mouse_pos.y, button);
    else
        mu_input_mouseup(ctx, ctx->mouse_pos.x, ctx->mouse_pos.y, button);
}

void poll_events(fb *dev, mu_Context *ctx, libinput *li, int fd) {
    std::array<struct pollfd, 1> pollFds{{fd, POLLIN, 0}};

    if (poll(pollFds.data(), pollFds.size(), -1) < 0)
        return;

    libinput_dispatch(li);

    for (struct libinput_event *event = libinput_get_event(li); event != nullptr; event = libinput_get_event(li)) {
        libinput_event_type eventType = libinput_event_get_type(event);

        if (eventType == LIBINPUT_EVENT_POINTER_MOTION)
            handlers::move(ctx, event, dev);
        else if (eventType == LIBINPUT_EVENT_POINTER_BUTTON)
            handlers::mousebtn(ctx, event, dev);

        libinput_event_destroy(event);
    }
}
