#ifndef BUOYANTUI_H
#define BUOYANTUI_H

// NOTE: services provided by the platform layer
typedef enum
{
    BUI_KEY_SPACE,
    BUI_KEY_APOSTROPHE,
    BUI_KEY_COMMA,
    BUI_KEY_MINUS,
    BUI_KEY_PERIOD,
    BUI_KEY_SLASH,
    BUI_KEY_0,
    BUI_KEY_1,
    BUI_KEY_2,
    BUI_KEY_3,
    BUI_KEY_4,
    BUI_KEY_5,
    BUI_KEY_6,
    BUI_KEY_7,
    BUI_KEY_8,
    BUI_KEY_9,
    BUI_KEY_SEMICOLON,
    BUI_KEY_EQUAL,
    BUI_KEY_A,
    BUI_KEY_B,
    BUI_KEY_C,
    BUI_KEY_D,
    BUI_KEY_E,
    BUI_KEY_F,
    BUI_KEY_G,
    BUI_KEY_H,
    BUI_KEY_I,
    BUI_KEY_J,
    BUI_KEY_K,
    BUI_KEY_L,
    BUI_KEY_M,
    BUI_KEY_N,
    BUI_KEY_O,
    BUI_KEY_P,
    BUI_KEY_Q,
    BUI_KEY_R,
    BUI_KEY_S,
    BUI_KEY_T,
    BUI_KEY_U,
    BUI_KEY_V,
    BUI_KEY_W,
    BUI_KEY_X,
    BUI_KEY_Y,
    BUI_KEY_Z,
    BUI_KEY_LEFT_BRACKET,
    BUI_KEY_BACKSLASH,
    BUI_KEY_RIGHT_BRACKET,
    BUI_KEY_GRAVE_ACCENT,
    BUI_KEY_WORLD_1,
    BUI_KEY_WORLD_2,
    BUI_KEY_ESCAPE,
    BUI_KEY_ENTER,
    BUI_KEY_TAB,
    BUI_KEY_BACKSPACE,
    BUI_KEY_INSERT,
    BUI_KEY_DELETE,
    BUI_KEY_RIGHT,
    BUI_KEY_LEFT,
    BUI_KEY_DOWN,
    BUI_KEY_UP,
    BUI_KEY_PAGE_UP,
    BUI_KEY_PAGE_DOWN,
    BUI_KEY_HOME,
    BUI_KEY_END,
    BUI_KEY_CAPS_LOCK,
    BUI_KEY_SCROLL_LOCK,
    BUI_KEY_NUM_LOCK,
    BUI_KEY_PRINT_SCREEN,
    BUI_KEY_PAUSE,
    BUI_KEY_F1,
    BUI_KEY_F2,
    BUI_KEY_F3,
    BUI_KEY_F4,
    BUI_KEY_F5,
    BUI_KEY_F6,
    BUI_KEY_F7,
    BUI_KEY_F8,
    BUI_KEY_F9,
    BUI_KEY_F10,
    BUI_KEY_F11,
    BUI_KEY_F12,
    BUI_KEY_F13,
    BUI_KEY_F14,
    BUI_KEY_F15,
    BUI_KEY_F16,
    BUI_KEY_F17,
    BUI_KEY_F18,
    BUI_KEY_F19,
    BUI_KEY_F20,
    BUI_KEY_F21,
    BUI_KEY_F22,
    BUI_KEY_F23,
    BUI_KEY_F24,
    BUI_KEY_F25,
    BUI_KEY_KP_0,
    BUI_KEY_KP_1,
    BUI_KEY_KP_2,
    BUI_KEY_KP_3,
    BUI_KEY_KP_4,
    BUI_KEY_KP_5,
    BUI_KEY_KP_6,
    BUI_KEY_KP_7,
    BUI_KEY_KP_8,
    BUI_KEY_KP_9,
    BUI_KEY_KP_DECIMAL,
    BUI_KEY_KP_DIVIDE,
    BUI_KEY_KP_MULTIPLY,
    BUI_KEY_KP_SUBTRACT,
    BUI_KEY_KP_ADD,
    BUI_KEY_KP_ENTER,
    BUI_KEY_KP_EQUAL,
    BUI_KEY_LEFT_SHIFT,
    BUI_KEY_LEFT_CONTROL,
    BUI_KEY_LEFT_ALT,
    BUI_KEY_LEFT_SUPER,
    BUI_KEY_RIGHT_SHIFT,
    BUI_KEY_RIGHT_CONTROL,
    BUI_KEY_RIGHT_ALT,
    BUI_KEY_RIGHT_SUPER,
    BUI_KEY_MENU,
    BUI_KEY_UNKNOWN
} Bui_Key;

typedef enum
{
    BUI_MOUSE_LEFT
} Bui_Mouse;

struct BuiKeyMods
{
    bool control;
    bool shift;
    bool alt;
};

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
internal void buoyantui_update(Buoyantui* buoyantui, float width, float height, float dt);
// TODO: make this accept bui key only and pass the other info via a bui interface
internal void buoyantui_key_pressed_callback(Bui_Key key, BuiKeyMods mods);

#endif // BUOYANTUI_H
