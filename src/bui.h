#ifndef BUI_H
#define BUI_H

typedef const char* Bui_Id;

typedef struct
{
    vec4 font;
    vec4 background;
    vec4 button;
    vec4 hovered;
} Bui_Color_Scheme;

typedef struct
{
    Arena*           arena;
    
    Renderer2D_Data  renderer;
    Texture2D        fontAtlas;

    Bui_Color_Scheme color_scheme;

    Bui_Id           active;
    Bui_Id           hovered;

    float            window_width;
    float            window_height;
    float            mouse_x;
    float            mouse_y;
} Bui;

static Bui* bui_init();
static void bui_begin_frame(Bui* bui, float width, float height);
static void bui_end_frame(Bui* bui);
static void bui_text(Bui* bui, const char* text, vec2 pos, vec2 size);
static bool bui_button(Bui* bui, const char* label, vec2 pos, vec2 size);

#endif // BUI_H
