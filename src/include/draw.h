#pragma once

#ifndef MUI_DRAW_H_INCLUDE
#define MUI_DRAW_H_INCLUDE

#include <cairo.h>

extern "C" {
#include "microui.h"
}

struct fb {
    unsigned int width;
    unsigned int height;
    void *buf;
    unsigned int len;
};

fb *open_fb();

inline cairo_surface_t *surface;
inline cairo_t *cr;

void rect(mu_RectCommand cmd);
void icon(mu_IconCommand cmd);
void text(mu_TextCommand cmd, const char *text);
void clip(mu_ClipCommand cmd);

// text fn
int text_width(mu_Font font, const char *str, int len);
int text_height(mu_Font font);

void init_draw(fb *dev);

void flush(mu_Context *ctx, fb *dev);

// other

void cursor(mu_Vec2 pos);

#endif
