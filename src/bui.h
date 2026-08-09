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

    Bui_Id           active;
    Bui_Id           hovered;
    
    Renderer2D_Data  renderer;
    Texture2D        fontAtlas;

    Bui_Color_Scheme color_scheme;

    float            font_size;
    float            item_padding;
    float            button_padding; // TODO: name???

    float            cursor_x;
    float            cursor_y;
    float            old_cursor_x;
    float            old_cursor_y;

    float            last_item_x;
    float            last_item_y;
    float            last_item_width;
    float            last_item_height;

    float            window_width;
    float            window_height;

    float            mouse_x;
    float            mouse_y;
    bool             mouse_down;
    bool             mouse_was_down;
} Bui;

internal Bui* bui_init();

internal void bui_begin_frame(Bui* bui, float width, float height);
internal void bui_end_frame(Bui* bui);

#define BUI_AUTO_LAYOUT
#ifdef BUI_AUTO_LAYOUT
internal void bui_set_next_pos(Bui* bui, vec2 pos);
internal void bui_same_line(Bui* bui);
internal void bui_text(Bui* bui, const char* text);
internal bool bui_button(Bui* bui, const char* label);
internal bool bui_checkbox(Bui* bui, const char* label, bool* checked);
#else
internal void bui_text(Bui* bui, const char* text, vec2 pos, vec2 size);
internal bool bui_button(Bui* bui, const char* label, vec2 pos, vec2 size);
#endif // BUI_AUTO_LAYOUT

#endif // BUI_H
