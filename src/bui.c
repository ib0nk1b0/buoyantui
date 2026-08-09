#include "bui.h"

internal bool bui_is_mouse_in_window(Bui* bui)
{
    return (bui->mouse_x >= 0                 &&
            bui->mouse_y >= 0                 &&
            bui->mouse_x <= bui->window_width &&
            bui->mouse_y <= bui->window_height);
}

internal bool bui_is_mouse_hovered(Bui* bui, vec2 pos, vec2 size)
{
    return (bui_is_mouse_in_window(bui)               &&
            bui->mouse_x >= pos[0] - (size[0] * 0.5f) &&
            bui->mouse_x <= pos[0] + (size[0] * 0.5f) &&
            bui->mouse_y >= pos[1] - (size[1] * 0.5f) &&
            bui->mouse_y <= pos[1] + (size[1] * 0.5f));
}

internal bool bui_is_active(Bui* bui, Bui_Id id)
{
    if (bui->active == NULL)
    {
        return false;
    }

    return strcmp(bui->active, id) == 0;
}

internal bool bui_is_hovered(Bui* bui, Bui_Id id)
{
    if (bui->hovered == NULL)
    {
        return false;
    }

    return strcmp(bui->hovered, id) == 0;
}

internal void bui_set_active(Bui* bui, Bui_Id id)
{
    // TODO:
    bui->active = id;
}

internal Bui* bui_init()
{
    Arena* arena = arena_create(Megabytes(64));
    Bui* bui = ArenaPushStruct(arena, Bui);
    bui->arena = arena;

    bui->renderer = renderer2D_init(arena);
    bui->fontAtlas = texture_create_from_file("resources\\fonts\\charmap-oldschool_white.png"); // TODO: relative paths

    bui->color_scheme = (Bui_Color_Scheme){
        .font       = { 0.90f, 0.90f, 0.95f, 1.0f },
        .background = { 0.10f, 0.12f, 0.22f, 1.0f },
        .button     = { 0.60f, 0.25f, 0.25f, 1.0f },
        .hovered    = { 0.25f, 0.60f, 0.25f, 1.0f },
    };

    bui->font_size = 14.0f;
    bui->item_padding = 16.0f;
    bui->button_padding = 16.0f;

    return bui;
}

internal void bui_begin_frame(Bui* bui, float width, float height)
{
    bui->window_width = width;
    bui->window_height = height;

    bui->cursor_x = bui->item_padding * 0.5f;
    bui->cursor_y = bui->window_height - (bui->font_size + bui->item_padding) * 0.5f;

    mat4 transform, view, projection, viewProjection;

    glm_mat4_identity(transform);

    glm_mat4_inv(transform, view);

    glm_ortho(0, width, 0, height, -1.0f, 1.0f, projection);
    glm_mat4_mul(projection, view, viewProjection);

    renderer2D_begin_scene(&bui->renderer, viewProjection, bui->window_width, bui->window_height, 0);
}

internal void bui_end_frame(Bui* bui)
{
    renderer2D_end_scene(&bui->renderer);
    bui->mouse_was_down = bui->mouse_down;
}

#ifdef BUI_AUTO_LAYOUT

internal void bui_move_cursor_down(Bui* bui, float extra_padding)
{
    bui->cursor_y -= bui->font_size + extra_padding + bui->item_padding * 0.5f;
    bui->cursor_x = (bui->item_padding) * 0.5f;
    // possibly remove ^
}

internal void bui_advance_cursor(Bui* bui, float extra_padding)
{
    // TODO: vertical and horizontal layouts
    bui_move_cursor_down(bui, extra_padding);

    if (bui->old_cursor_x != 0.0f || bui->old_cursor_y != 0.0f)
    {
        bui->cursor_x = bui->old_cursor_x;
        bui->cursor_y = bui->old_cursor_y;

        bui->old_cursor_x = 0.0f;
        bui->old_cursor_y = 0.0f;
    }
}

internal void bui_set_next_pos(Bui* bui, vec2 pos)
{
    bui->old_cursor_x = bui->cursor_x;
    bui->old_cursor_y = bui->cursor_y;

    bui->cursor_x = pos[0];
    bui->cursor_y = pos[1];
}

internal void bui_same_line(Bui* bui)
{
    bui->cursor_x = bui->last_item_x + (bui->last_item_width + bui->item_padding * 0.5f);
    bui->cursor_y = bui->last_item_y;
}

internal void bui_text(Bui* bui, const char* text)
{
    mat4 transform = GLM_MAT4_IDENTITY_INIT;

    vec2 size = { bui->font_size, bui->font_size };
    vec2 pos = { bui->cursor_x + size[0] * 0.5f, bui->cursor_y };

    // TODO:
    bui->last_item_x = bui->cursor_x;
    bui->last_item_y = bui->cursor_y;
    bui->last_item_width = size[0] * (float)strlen(text);
    bui->last_item_height = size[1];

    glm_translate_x(transform, pos[0]);
    glm_translate_y(transform, pos[1]);

    glm_scale(transform, size);

    renderer2D_draw_string(&bui->renderer, bui->fontAtlas, text, transform, bui->color_scheme.font);

    bui_advance_cursor(bui, 0.0f);
}

internal bool bui_button(Bui* bui, const char* label)
{
    bool result = false;

    vec2 size = { bui->font_size * strlen(label) + bui->button_padding, bui->font_size + bui->button_padding };

    vec2 pos = { bui->cursor_x + size[0] * 0.5f, bui->cursor_y - bui->button_padding * 0.5f };

    // TODO:
    bui->last_item_x = bui->cursor_x;
    bui->last_item_y = bui->cursor_y;
    bui->last_item_width = size[0];
    bui->last_item_height = size[1];

    bui_advance_cursor(bui, bui->button_padding);

    if (bui_is_active(bui, label))
    {
        if (bui->mouse_was_down && !bui->mouse_down) // NOTE: mouse went up
        {
            if (bui_is_hovered(bui, label))
            {
                bui->active = NULL;
                result = true;
            }
            bui->active = NULL;
        }
    }
    else if (bui_is_hovered(bui, label))
    {
        if (!bui->mouse_was_down && bui->mouse_down) // NOTE: mouse went down
        {
            bui_set_active(bui, label);
        }
    }

    vec4 color;
    if (bui_is_mouse_hovered(bui, pos, size))
    {
        glm_vec4_copy(bui->color_scheme.hovered, color);
        bui->hovered = label;
    }
    else
    {
        glm_vec4_copy(bui->color_scheme.button, color);
        if (bui->hovered)
        {
            if (strcmp(bui->hovered, label) == 0)
            {
                bui->hovered = NULL;
            }
        }
    }

    mat4 transform = GLM_MAT4_IDENTITY_INIT;

    glm_translate_x(transform, pos[0]);
    glm_translate_y(transform, pos[1]);

    glm_scale(transform, size);

    renderer2D_draw_quad(&bui->renderer, transform, color);

    if (strlen(label) >= 2)
    {
        if (label[0] == '#' && label[1] == '#')
        {
            // NOTE: just use as ID
            return result;
        }
    }

    mat4 label_transform = GLM_MAT4_IDENTITY_INIT;

    glm_translate_x(label_transform, pos[0] - ((float)strlen(label) * bui->font_size - bui->button_padding) * 0.5f);
    glm_translate_y(label_transform, pos[1]);
    glm_scale(label_transform, (vec2){ bui->font_size, bui->font_size });

    renderer2D_draw_string(&bui->renderer, bui->fontAtlas, label, label_transform, bui->color_scheme.font);

    return result;
}

// TODO: 
internal bool bui_checkbox(Bui* bui, const char* label, bool* checked)
{
    bool result = false;
    
    vec2 pos = { bui->cursor_x, bui->cursor_y };
    vec2 size = { bui->font_size, bui->font_size };

    // TODO:
    bui->last_item_x = bui->cursor_x;
    bui->last_item_y = bui->cursor_y;
    bui->last_item_width = size[0];
    bui->last_item_height = size[1];

    bui_advance_cursor(bui, 0.0f);

    return result;
}

#else
internal void bui_text(Bui* bui, const char* text, vec2 pos, vec2 size)
{
    mat4 transform = GLM_MAT4_IDENTITY_INIT;

    glm_translate_x(transform, pos[0]);
    glm_translate_y(transform, pos[1]);

    glm_scale(transform, size);

    renderer2D_draw_string(&bui->renderer, bui->fontAtlas, text, transform, bui->color_scheme.font);
}

internal bool bui_button(Bui* bui, const char* label, vec2 pos, vec2 size)
{
    // TODO: error checking

    /*
     * if active
     *      if mouse up
     *          if hovered
     *              return true
     *          unset active
     *          return false
     * if hovered
     *      if mouse down
     *          set active
     *
     * if inside
     * */

    if (bui_is_active(bui, label))
    {
        if (bui->mouse_was_down && !bui->mouse_down) // NOTE: mouse went up
        {
            if (bui_is_hovered(bui, label))
            {
                bui->active = NULL;
                return true;
            }
            bui->active = NULL;
        }
    }
    else if (bui_is_hovered(bui, label))
    {
        if (!bui->mouse_was_down && bui->mouse_down) // NOTE: mouse went down
        {
            bui_set_active(bui, label);
        }
    }

    vec4 color;
    if (bui_is_mouse_hovered(bui, pos, size))
    {
        glm_vec4_copy(bui->color_scheme.hovered, color);
        bui->hovered = label;
    }
    else
    {
        glm_vec4_copy(bui->color_scheme.button, color);
        if (bui->hovered)
        {
            if (strcmp(bui->hovered, label) == 0)
            {
                bui->hovered = NULL;
            }
        }
    }

    mat4 transform = GLM_MAT4_IDENTITY_INIT;

    glm_translate_x(transform, pos[0]);
    glm_translate_y(transform, pos[1]);

    glm_scale(transform, size);

    renderer2D_draw_quad(&bui->renderer, transform, color);

    if (strlen(label) >= 2)
    {
        if (label[0] == '#' && label[1] == '#')
        {
            // NOTE: just use as ID
            return false;
        }
    }

    // TODO: auto scaling for text length
    glm_translate_x(transform, -0.42f);
    glm_scale(transform, (vec2){1.0f / (float)strlen(label), 0.8f });

    renderer2D_draw_string(&bui->renderer, bui->fontAtlas, label, transform, bui->color_scheme.font);

    return false;
}
#endif // BUI_AUTO_LAYOUT

