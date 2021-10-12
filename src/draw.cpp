
#include <iostream>
#include <string.h>
#include <string>

#include <cairo.h>

extern "C" {
#include "microui.h"
}
#include "include/draw.h"

#define FONT_SIZE 12

void rect(mu_RectCommand cmd) {
    cairo_set_source_rgba(cr, (double)cmd.color.r / 255, (double)cmd.color.g / 255, (double)cmd.color.b / 255, (double)cmd.color.a / 255);
    cairo_rectangle(cr, cmd.rect.x, cmd.rect.y, cmd.rect.w, cmd.rect.h);

    cairo_fill(cr);
}

void icon(mu_IconCommand cmd) {
}

void text(mu_TextCommand cmd, const char *text) {

    cairo_select_font_face(cr, "raleway", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, FONT_SIZE);
    cairo_set_source_rgba(cr, (double)cmd.color.r / 255, (double)cmd.color.g / 255, (double)cmd.color.b / 255, (double)cmd.color.a / 255);
    cairo_move_to(cr, cmd.pos.x, cmd.pos.y + text_height(cmd.font) - 1);
    cairo_text_path(cr, text);

    cairo_fill(cr);
}

void clip(mu_ClipCommand cmd) {
    cairo_rectangle(cr, cmd.rect.x, cmd.rect.y, cmd.rect.w, cmd.rect.h);
    cairo_clip(cr);
}

int text_width(mu_Font font, const char *str, int len) {
    if (len == -1)
        len = strlen(str);

    cairo_select_font_face(cr, "raleway", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12.);

    cairo_text_extents_t extents;
    cairo_text_extents(cr, str, &extents);

    return extents.width;
}

int text_height(mu_Font font) {
    return FONT_SIZE;
}

void init_draw(fb *dev) {
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, dev->width, dev->height);
    cr = cairo_create(surface);
}

void flush(mu_Context *ctx, fb *dev) {
    mu_Command *cmd = nullptr;

    while (mu_next_command(ctx, &cmd)) {
        if (cmd->type == MU_COMMAND_TEXT)
            text(cmd->text, cmd->text.str);

        else if (cmd->type == MU_COMMAND_RECT)
            rect(cmd->rect);

        else if (cmd->type == MU_COMMAND_ICON)
            icon(cmd->icon);

        else if (cmd->type == MU_COMMAND_CLIP)
            clip(cmd->clip);
    }

    cursor(ctx->mouse_pos);

    cairo_surface_flush(surface);

    auto source = cairo_image_surface_get_data(surface);
    memcpy(dev->buf, source, dev->len);
}

void cursor(mu_Vec2 pos) {
    cairo_set_source_rgba(cr, .8, .8, .8, 1);
    cairo_rectangle(cr, pos.x - 1, pos.y - 1, 6, 6);
    cairo_fill(cr);

    cairo_set_source_rgba(cr, .2, .2, .2, 1);
    cairo_rectangle(cr, pos.x, pos.y, 4, 4);
    cairo_fill(cr);
}
