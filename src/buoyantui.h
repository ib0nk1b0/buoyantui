#ifndef BUOYANTUI_H
#define BUOYANTUI_H

// NOTE: services provided by the platform layer
typedef enum
{
    BUI_KEY_W
} Bui_Key;

typedef enum
{
    BUI_MOUSE_LEFT
} Bui_Mouse;

struct Buoyantui
{
    Arena*           arena;
    Renderer2D_Data* renderer;
    Bui*             bui;
    Texture2D        tilemap;
    int              map_width;
    int              map_height;
    int              tile_size;
    int**            tiles;
};

// NOTE: services provided from the platform layer
internal bool platform_input_is_key_down(Bui_Key key);
internal void platform_input_get_mouse_pos(double* mouse_x, double* mouse_y);
internal bool platform_input_is_mouse_down(Bui_Mouse button);

// NOTE: services provided to the platform layer
internal void buoyantui_update(Buoyantui* buoyantui, float width, float height);
internal void buoyantui_key_callback(int key, int scancode, int action, int mods);

#endif // BUOYANTUI_H
