#include "bui.h"
#include "buoyantui.h"

// Actual window not bui window...
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

internal bool bui_item_clicked(Bui* bui, Bui_Id label)
{
    bool result = false;
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

    return result;
}

internal Bui* bui_init()
{
    Arena* arena = arena_create(Megabytes(64));
    Bui* bui = ArenaPushStruct(arena, Bui);
    bui->arena = arena;

    bui->renderer = renderer2D_init(arena);
    bui->fontAtlas = texture_create_from_file("resources\\fonts\\charmap-oldschool_white.png");

    bui->color_scheme = (Bui_Color_Scheme){
        .font              = { 0.90f, 0.90f, 0.95f, 1.0f },
        .background        = { 0.04f, 0.06f, 0.12f, 1.0f },
        .header_background = { 0.08f, 0.12f, 0.24f, 1.0f },
        .button            = { 0.01f, 0.17f, 0.75f, 1.0f },
        .hovered           = { 0.46f, 0.57f, 1.00f, 1.0f },
    };

    bui->font_size = 11.0f;
    bui->item_padding = 16.0f;
    bui->button_padding = 16.0f;

    bui->current_window = NULL;

    return bui;
}

internal void bui_begin_frame(Bui* bui, float width, float height)
{
    bui->window_width = width;
    bui->window_height = height;

    bui->cursor_x = bui->item_padding * 0.5f;
    bui->cursor_y = bui->window_height - (bui->font_size + bui->item_padding) * 0.5f;

    double x = 0.0f;
    double y = 0.0f;
    platform_input_get_mouse_pos(&x, &y);
    bui->mouse_x = (float)x;
    bui->mouse_y = (float)y;

    bui->mouse_down = platform_input_is_mouse_down(BUI_MOUSE_LEFT);

    mat4 transform, view, projection, viewProjection;

    glm_mat4_identity(transform);

    glm_mat4_inv(transform, view);

    glm_ortho(0, width, 0, height, -1.0f, 1.0f, projection);
    glm_mat4_mul(projection, view, viewProjection);

    renderer2D_begin_scene(&bui->renderer, viewProjection);
}

internal void bui_end_frame(Bui* bui)
{
    bui->mouse_was_down = bui->mouse_down;
    renderer2D_end_scene(&bui->renderer);
}

internal void bui_begin_window(Bui* bui, const char* label, int x, int y, int width, int height)
{
    if (bui->current_window != NULL)
    {
        printf("Only support one window at a time\n");
        assert(false);
        return;
    }

    bui->current_window = label;
    bui->current_window_x = x;
    bui->current_window_y = y;
    bui->current_window_width = width;
    bui->current_window_height = height;

    bui->cursor_x_return = bui->cursor_x;
    bui->cursor_y_return = bui->cursor_y;

    float header_size = bui->font_size + bui->item_padding * 2.0f;
    bui->cursor_x = (float)x + bui->item_padding * 0.5f;
    bui->cursor_y = (float)y + (float)height - (bui->font_size + bui->item_padding + header_size) * 0.5f;

    vec2 pos = { (float)x + (float)width * 0.5f, (float)y + (float)height * 0.5f };
    vec2 size = { (float)width, (float)height };
    
    // hovered logic
    if (bui_is_mouse_hovered(bui, pos, size))
    {
        bui->hovered = label;
    }
    else
    {
        if (bui->hovered && strcmp(bui->hovered, label) == 0)
        {
            bui->hovered = NULL;
        }
    }

    // render whatever widgets were already in the buffer
    renderer2D_flush(&bui->renderer);

    // diable depth test
    renderer_api_disable_depth_test();

    // start the scissor for this window
    renderer_api_enable_scissor();
    renderer_api_scissor(x - 1, y - 1, width + 1, height + 1);

    // Draw backgound and border
    mat4 transform = GLM_MAT4_IDENTITY_INIT;

    glm_translate_x(transform, pos[0]);
    glm_translate_y(transform, pos[1]);
    // glm_translate_z(transform, -0.5f); // redundent if depth test is disabled

    glm_scale(transform, size);

    renderer2D_draw_quad(&bui->renderer, transform, bui->color_scheme.background);
    renderer2D_draw_rect(&bui->renderer, transform, bui->color_scheme.font);

    // Draw the header
    mat4 header_transform = GLM_MAT4_IDENTITY_INIT;

    glm_translate_x(header_transform, pos[0]);
    glm_translate_y(header_transform, pos[1] + (size[1] * 0.5f));

    glm_scale(header_transform, (vec3){ size[0], header_size, 1.0f });

    renderer2D_draw_quad(&bui->renderer, header_transform, bui->color_scheme.header_background);
    renderer2D_draw_rect(&bui->renderer, header_transform, bui->color_scheme.font);

    mat4 title_transform = GLM_MAT4_IDENTITY_INIT;
    glm_translate_x(title_transform, pos[0] - size[0] * 0.5f + bui->item_padding);
    glm_translate_y(title_transform, pos[1] + (size[1] - bui->item_padding - bui->font_size) * 0.5f);
    // glm_translate_z(title_transform, 0.5f); // redundent if depth test is disabled

    glm_scale(title_transform, (vec3){ bui->font_size, bui->font_size, 1.0f });

    renderer2D_draw_string(&bui->renderer, bui->fontAtlas, label, title_transform, bui->color_scheme.font);
}

internal void bui_end_window(Bui* bui)
{
    bui->current_window = NULL;
    bui->cursor_x = bui->cursor_x_return;
    bui->cursor_y = bui->cursor_y_return;
    renderer2D_flush(&bui->renderer);
    renderer_api_disable_scissor();
    renderer_api_enable_depth_test();
}

internal void bui_move_cursor_down(Bui* bui, float extra_padding)
{
    bui->cursor_y -= bui->font_size + extra_padding + bui->item_padding * 0.5f;
    bui->cursor_x = bui->item_padding * 0.5f;
    if (bui->current_window != NULL)
    {
        bui->cursor_x += bui->current_window_x;
    }
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

    result = bui_item_clicked(bui, label);

    mat4 transform = GLM_MAT4_IDENTITY_INIT;

    glm_translate_x(transform, pos[0]);
    glm_translate_y(transform, pos[1]);

    glm_scale(transform, size);

    renderer2D_draw_quad(&bui->renderer, transform, color);
    renderer2D_draw_rect(&bui->renderer, transform, bui->color_scheme.font);

    if (strlen(label) >= 2)
    {
        if (label[0] == '#' && label[1] == '#')
        {
            // NOTE: just use as ID
            return result;
        }
    }

    mat4 label_transform = GLM_MAT4_IDENTITY_INIT;

    glm_translate_x(label_transform, pos[0] - (size[0] * 0.5f) + (bui->button_padding + bui->font_size) * 0.5f);
    glm_translate_y(label_transform, pos[1]);
    glm_translate_z(label_transform, 0.5f); // NOTE: to ensure on top with depth buffer enabled
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

