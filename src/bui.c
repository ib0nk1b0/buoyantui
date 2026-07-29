#include "bui.h"

internal bool bui_is_hovered(Bui* bui, vec2 pos, vec2 size)
{
    if (bui->mouse_x >= pos[0] - (size[0] * 0.5f) &&
        bui->mouse_x <= pos[0] + (size[0] * 0.5f) &&
        bui->mouse_y >= pos[1] - (size[1] * 0.5f) &&
        bui->mouse_y <= pos[1] + (size[1] * 0.5f) &&
        bui->mouse_x >= 0 &&
        bui->mouse_x <= bui->window_width &&
        bui->mouse_y >= 0 &&
        bui->mouse_y <= bui->window_height)
    {
        return true;
    }

    return false;
}

static Bui* bui_init()
{
    Arena* arena = arena_create(Megabytes(64));
    Bui* bui = ArenaPushStruct(arena, Bui);
    bui->arena = arena;

    bui->renderer = renderer2D_init(arena);
    bui->fontAtlas = texture_create_from_file("W:\\buoyant\\resources\\fonts\\charmap-oldschool_white.png"); // TODO: relative paths

    bui->color_scheme = (Bui_Color_Scheme){
        .font       = { 0.90f, 0.90f, 0.95f, 1.0f },
        .background = { 0.10f, 0.12f, 0.22f, 1.0f },
        .button     = { 0.60f, 0.25f, 0.25f, 1.0f },
        .hovered    = { 0.25f, 0.60f, 0.25f, 1.0f },
    };

    return bui;
}

static void bui_begin_frame(Bui* bui, float width, float height)
{
    bui->window_width = width;
    bui->window_height = height;

    mat4 transform, view, projection, viewProjection;

    glm_mat4_identity(transform);

    glm_mat4_inv(transform, view);

    glm_ortho(0, width, 0, height, -1.0f, 1.0f, projection);
    glm_mat4_mul(projection, view, viewProjection);

    renderer2D_begin_scene(&bui->renderer, viewProjection, bui->window_width, bui->window_height, 0);
}

static void bui_end_frame(Bui* bui)
{
    renderer2D_end_scene(&bui->renderer);
}

static void bui_text(Bui* bui, const char* text, vec2 pos, vec2 size)
{
    mat4 transform = GLM_MAT4_IDENTITY_INIT;

    glm_translate_x(transform, pos[0]);
    glm_translate_y(transform, pos[1]);

    glm_scale(transform, size);

    renderer2D_draw_string(&bui->renderer, bui->fontAtlas, text, transform, bui->color_scheme.font);
}

static bool bui_button(Bui* bui, const char* label, vec2 pos, vec2 size)
{
    // TODO: error checking

    vec4 color;
    if (bui_is_hovered(bui, pos, size))
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

    // TODO: auto scaling for text length
    glm_translate_x(transform, -0.42f);
    glm_scale(transform, (vec2){1.0f / (float)strlen(label), 0.8f });

    renderer2D_draw_string(&bui->renderer, bui->fontAtlas, label, transform, bui->color_scheme.font);

    return false;
}

