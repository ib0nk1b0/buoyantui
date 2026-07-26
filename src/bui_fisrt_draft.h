#ifndef BUI_H
#define BUI_H

#define BUI_MAX_UI_ELEMENTS 1024

typedef enum
{
    BUI_WINDOW = 0,
    BUI_BUTTON,
} Element_Kind;

typedef struct
{
    size_t id;
    size_t parent;
    const char* name;
    Element_Kind kind;
} BUI_Element;

typedef struct
{
    size_t active;
    size_t hovered;
    BUI_Element* elements;
    size_t element_count;
    size_t current_parent;
    Renderer2D_Data renderer;
    Texture2D fontAtlas;
} BUI;

BUI* bui_init(Arena* arena);
void bui_begin_frame(BUI* bui, float width, float height);
void bui_end_frame(BUI* bui);

void bui_begin(BUI* bui, const char* name, vec2 pos, vec2 size);
void bui_end(BUI* bui);

bool bui_button(BUI* bui, const char* name, vec2 pos, vec2 size);

#endif // BUI_H


#define BUI_IMPLEMENTATION // TODO: temp for syntax highlighting
#ifdef BUI_IMPLEMENTATION

BUI* bui_init(Arena* arena)
{
    BUI* bui = ArenaPushStruct(arena, BUI);
    bui->elements = ArenaPushArray(arena, BUI_Element, BUI_MAX_UI_ELEMENTS);
    bui->element_count = 0; // NOTE: bui->element_count - 1 == last element
    bui->renderer = renderer2D_init(arena);
    bui->fontAtlas = texture_create_from_file("W:\\buoyant\\resources\\textures\\charmap-oldschool_white.png");

    return bui;
}

void bui_begin_frame(BUI* bui, float width, float height)
{
    mat4 transform, view, projection, viewProjection;

    glm_mat4_identity(transform);
    glm_translate_x(transform, (float)-width * 0.5f);
    glm_translate_y(transform, (float)-height * 0.5f);

    glm_mat4_inv(transform, view);

    glm_ortho(0, width, 0, height, -1.0f, 1.0f, projection);
    glm_mat4_mul(projection, view, viewProjection);

    renderer2D_begin_scene(&bui->renderer, viewProjection, width, height, 0);
}

void bui_end_frame(BUI* bui)
{
    renderer2D_end_scene(&bui->renderer);
    memset(bui->elements, 0, BUI_MAX_UI_ELEMENTS * sizeof(BUI_Element));
    bui->element_count = 0;
}

void bui_begin(BUI* bui, const char* name, vec2 pos, vec2 size)
{
    // create a new window with name "name"

    if (bui->element_count <= BUI_MAX_UI_ELEMENTS)
    {
        BUI_Element window = {
            .id = bui->element_count,
            .parent = bui->current_parent,
            .name = name,
            .kind = BUI_WINDOW,
        };

        bui->current_parent = window.id;

        bui->elements[bui->element_count++] = window;

        // Draw window

        // TODO: temp
        vec4 color_white = { 1.0f, 1.0f, 1.0f, 1.0f };
        vec4 color_red   = { 0.8f, 0.3f, 0.2f, 1.0f };
        vec4 color_green = { 0.2f, 0.8f, 0.3f, 1.0f };
        vec4 color_blue  = { 0.2f, 0.3f, 0.8f, 1.0f };

        vec2 header_size = { size[0],      size[1]*0.07f };
        vec2 text_size =   { size[0]*0.1f, size[1]*0.05f };

        mat4 transform, header, text;
        glm_mat4_identity(transform);
        glm_mat4_identity(header);
        glm_mat4_identity(text);

        glm_translate_x(transform, pos[0]);
        glm_translate_x(header, pos[0] - size[0] * 0.5f + header_size[0] * 0.5f);
        glm_translate_x(text, pos[0] - size[0] * 0.5f + text_size[0] * 0.75f);

        glm_translate_y(transform, pos[1]);
        glm_translate_y(header, pos[1] + size[1] * 0.5f - header_size[1] * 0.5f);
        glm_translate_y(text, pos[1] + size[1] * 0.5f - header_size[1] * 0.5f);

        glm_scale(transform, size);
        glm_scale(header, header_size);
        glm_scale(text, text_size);

        renderer2D_draw_quad(&bui->renderer, transform, color_blue);
        renderer2D_draw_quad(&bui->renderer, header, color_red);
        renderer2D_draw_string(&bui->renderer, bui->fontAtlas, window.name, text, color_white);
    }
    else
    {
        printf("[ERROR]: Attempted to open a window when max UI elements are in use\n");
    }
}

void bui_end(BUI* bui)
{
    if (bui->element_count > 0)
    {
        bui->current_parent = bui->elements[bui->element_count - 1].parent;
    }
    else
    {
        printf("[ERROR]: Call to bui_end() with no call to bui_begin\n");
    }
}

bool bui_button(BUI* bui, const char* name, vec2 pos, vec2 size)
{

    if (bui->element_count <= BUI_MAX_UI_ELEMENTS)
    {
        BUI_Element button = {
            .id = bui->element_count,
            .parent = bui->current_parent,
            .name = name,
            .kind = BUI_BUTTON,
        };

        bui->elements[bui->element_count++] = button;

        mat4 transform;
        glm_mat4_identity(transform);
        glm_translate_x(transform, pos[0]);
        glm_translate_y(transform, pos[1]);
        glm_scale(transform, size);
        vec4 color = { 0.6f, 0.1f, 0.8f, 1.0f };
        renderer2D_draw_quad(&bui->renderer, transform, color);
        glm_translate_x(transform, -0.4f);
        glm_scale(transform, (vec2){1.0f/(float)strlen(name), 0.8f});
        renderer2D_draw_string(&bui->renderer, bui->fontAtlas, name, transform, (vec4){1.0f, 1.0f, 1.0f, 1.0f});
    }

    return false;
}

#endif // BUI_IMPLEMENTATION
