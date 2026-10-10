#ifndef BUOYANTUI_H
#define BUOYANTUI_H

// NOTE: services provided by the platform layer
typedef enum
{
    BUI_KEY_SPACE         = ' ',
    BUI_KEY_APOSTROPHE    = '\'',
    BUI_KEY_COMMA         = ',',
    BUI_KEY_MINUS         = '-',
    BUI_KEY_PERIOD        = '.',
    BUI_KEY_SLASH         = '/',
    BUI_KEY_0             = '0',
    BUI_KEY_1             = '1',
    BUI_KEY_2             = '2',
    BUI_KEY_3             = '3',
    BUI_KEY_4             = '4',
    BUI_KEY_5             = '5',
    BUI_KEY_6             = '6',
    BUI_KEY_7             = '7',
    BUI_KEY_8             = '8',
    BUI_KEY_9             = '9',
    BUI_KEY_SEMICOLON     = ';',
    BUI_KEY_EQUAL         = '=',
    BUI_KEY_A             = 'A',
    BUI_KEY_B             = 'B',
    BUI_KEY_C             = 'C',
    BUI_KEY_D             = 'D',
    BUI_KEY_E             = 'E',
    BUI_KEY_F             = 'F',
    BUI_KEY_G             = 'G',
    BUI_KEY_H             = 'H',
    BUI_KEY_I             = 'I',
    BUI_KEY_J             = 'J',
    BUI_KEY_K             = 'K',
    BUI_KEY_L             = 'L',
    BUI_KEY_M             = 'M',
    BUI_KEY_N             = 'N',
    BUI_KEY_O             = 'O',
    BUI_KEY_P             = 'P',
    BUI_KEY_Q             = 'Q',
    BUI_KEY_R             = 'R',
    BUI_KEY_S             = 'S',
    BUI_KEY_T             = 'T',
    BUI_KEY_U             = 'U',
    BUI_KEY_V             = 'V',
    BUI_KEY_W             = 'W',
    BUI_KEY_X             = 'X',
    BUI_KEY_Y             = 'Y',
    BUI_KEY_Z             = 'Z',
    BUI_KEY_LEFT_BRACKET  = '[',
    BUI_KEY_BACKSLASH     = '\\',
    BUI_KEY_RIGHT_BRACKET = ']',
    BUI_KEY_GRAVE_ACCENT  = '`',

    BUI_KEY_WORLD_1       = 256,
    BUI_KEY_WORLD_2       = 257,
    BUI_KEY_ESCAPE        = 258,
    BUI_KEY_ENTER         = 259,
    BUI_KEY_TAB           = 260,
    BUI_KEY_BACKSPACE     = 261,
    BUI_KEY_INSERT        = 262,
    BUI_KEY_DELETE        = 263,
    BUI_KEY_RIGHT         = 264,
    BUI_KEY_LEFT          = 265,
    BUI_KEY_DOWN          = 266,
    BUI_KEY_UP            = 267,
    BUI_KEY_PAGE_UP       = 268,
    BUI_KEY_PAGE_DOWN     = 269,
    BUI_KEY_HOME          = 270,
    BUI_KEY_END           = 271,
    BUI_KEY_CAPS_LOCK     = 272,
    BUI_KEY_SCROLL_LOCK   = 273,
    BUI_KEY_NUM_LOCK      = 274,
    BUI_KEY_PRINT_SCREEN  = 275,
    BUI_KEY_PAUSE         = 276,
    BUI_KEY_F1            = 277,
    BUI_KEY_F2            = 278,
    BUI_KEY_F3            = 279,
    BUI_KEY_F4            = 280,
    BUI_KEY_F5            = 281,
    BUI_KEY_F6            = 282,
    BUI_KEY_F7            = 283,
    BUI_KEY_F8            = 284,
    BUI_KEY_F9            = 285,
    BUI_KEY_F10           = 286,
    BUI_KEY_F11           = 287,
    BUI_KEY_F12           = 288,
    BUI_KEY_F13           = 289,
    BUI_KEY_F14           = 290,
    BUI_KEY_F15           = 291,
    BUI_KEY_F16           = 292,
    BUI_KEY_F17           = 293,
    BUI_KEY_F18           = 294,
    BUI_KEY_F19           = 295,
    BUI_KEY_F20           = 296,
    BUI_KEY_F21           = 297,
    BUI_KEY_F22           = 298,
    BUI_KEY_F23           = 299,
    BUI_KEY_F24           = 300,
    BUI_KEY_F25           = 301,
    BUI_KEY_KP_0          = 302,
    BUI_KEY_KP_1          = 303,
    BUI_KEY_KP_2          = 304,
    BUI_KEY_KP_3          = 305,
    BUI_KEY_KP_4          = 306,
    BUI_KEY_KP_5          = 307,
    BUI_KEY_KP_6          = 308,
    BUI_KEY_KP_7          = 309,
    BUI_KEY_KP_8          = 310,
    BUI_KEY_KP_9          = 311,
    BUI_KEY_KP_DECIMAL    = 312,
    BUI_KEY_KP_DIVIDE     = 313,
    BUI_KEY_KP_MULTIPLY   = 314,
    BUI_KEY_KP_SUBTRACT   = 315,
    BUI_KEY_KP_ADD        = 316,
    BUI_KEY_KP_ENTER      = 317,
    BUI_KEY_KP_EQUAL      = 318,
    BUI_KEY_LEFT_SHIFT    = 319,
    BUI_KEY_LEFT_CONTROL  = 320,
    BUI_KEY_LEFT_ALT      = 321,
    BUI_KEY_LEFT_SUPER    = 322,
    BUI_KEY_RIGHT_SHIFT   = 323,
    BUI_KEY_RIGHT_CONTROL = 324,
    BUI_KEY_RIGHT_ALT     = 325,
    BUI_KEY_RIGHT_SUPER   = 326,
    BUI_KEY_MENU          = 327,

    BUI_KEY_UNKNOWN       = -1
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

enum class BuiKeyAction {
    Press,
    Repeat,
    Release
};

struct BuiKeyEvent {
    BuiKeyAction action;
    Bui_Key key;
    BuiKeyMods mods;
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
    int*             tiles;
};

// NOTE: services provided from the platform layer
internal bool platform_input_is_key_down(Bui_Key key);
internal void platform_input_get_mouse_pos(double* mouse_x, double* mouse_y);
internal bool platform_input_is_mouse_down(Bui_Mouse button);

// NOTE: services provided to the platform layer
internal void buoyantui_update(Buoyantui* buoyantui, float width, float height, float dt);
// TODO: make this accept bui key only and pass the other info via a bui interface
internal void buoyantui_key_event_callback(BuiKeyEvent event);

#endif // BUOYANTUI_H
