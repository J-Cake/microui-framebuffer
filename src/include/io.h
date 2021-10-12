#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <functional>
#include <libinput.h>
#include <map>
#include <vector>

#include "draw.h"
#include "microui.h"

void init_event(fb *dev, mu_Context *ctx);
void poll_events(fb *dev, mu_Context *ctx, libinput *li, int fd);

namespace handlers {

void move(mu_Context *ctx, libinput_event *event, fb *dev);
void mousebtn(mu_Context *ctx, libinput_event *event, fb *dev);

} // namespace handlers

#endif
