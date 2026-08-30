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

internal bool platform_input_is_key_down(Bui_Key key);
internal void platform_input_get_mouse_pos(double* mouse_x, double* mouse_y);
internal bool platform_input_is_mouse_down(Bui_Mouse button);

// NOTE: services provided to the platform layer
internal void buoyantui_update(Arena* arena, Bui* bui, float width, float height);

#endif // BUOYANTUI_H
