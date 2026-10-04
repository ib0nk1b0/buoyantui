#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <Windows.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// #define CGLM_IMPLEMENTATION
// #define CGLM_ALL_UNALIGNED
// #include <cglm/cglm.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

// #include <ft2build.h>
// #include FT_FREETYPE_H  

// My includes
#include "utils.h"

#include "renderer/opengl_buffer.cpp"
#include "renderer/opengl_shader.cpp"
#include "renderer/opengl_texture.cpp"
#include "renderer/opengl_renderer_api.cpp"
#include "renderer/opengl_renderer.cpp"

#include "bui.cpp"

#include "calculator.cpp"

#include "buoyantui.cpp"

global GLFWwindow* g_Window = NULL;
global uint32_t    g_Width = 0;
global uint32_t    g_Height = 0;

internal Bui_Key glfw_key_to_bui(int key)
{
    switch(key)
    {
        case GLFW_KEY_SPACE:         return BUI_KEY_SPACE;
        case GLFW_KEY_APOSTROPHE:    return BUI_KEY_APOSTROPHE;
        case GLFW_KEY_COMMA:         return BUI_KEY_COMMA;
        case GLFW_KEY_MINUS:         return BUI_KEY_MINUS;
        case GLFW_KEY_PERIOD:        return BUI_KEY_PERIOD;
        case GLFW_KEY_SLASH:         return BUI_KEY_SLASH;
        case GLFW_KEY_0:             return BUI_KEY_0;
        case GLFW_KEY_1:             return BUI_KEY_1;
        case GLFW_KEY_2:             return BUI_KEY_2;
        case GLFW_KEY_3:             return BUI_KEY_3;
        case GLFW_KEY_4:             return BUI_KEY_4;
        case GLFW_KEY_5:             return BUI_KEY_5;
        case GLFW_KEY_6:             return BUI_KEY_6;
        case GLFW_KEY_7:             return BUI_KEY_7;
        case GLFW_KEY_8:             return BUI_KEY_8;
        case GLFW_KEY_9:             return BUI_KEY_9;
        case GLFW_KEY_SEMICOLON:     return BUI_KEY_SEMICOLON;
        case GLFW_KEY_EQUAL:         return BUI_KEY_EQUAL;
        case GLFW_KEY_A:             return BUI_KEY_A;
        case GLFW_KEY_B:             return BUI_KEY_B;
        case GLFW_KEY_C:             return BUI_KEY_C;
        case GLFW_KEY_D:             return BUI_KEY_D;
        case GLFW_KEY_E:             return BUI_KEY_E;
        case GLFW_KEY_F:             return BUI_KEY_F;
        case GLFW_KEY_G:             return BUI_KEY_G;
        case GLFW_KEY_H:             return BUI_KEY_H;
        case GLFW_KEY_I:             return BUI_KEY_I;
        case GLFW_KEY_J:             return BUI_KEY_J;
        case GLFW_KEY_K:             return BUI_KEY_K;
        case GLFW_KEY_L:             return BUI_KEY_L;
        case GLFW_KEY_M:             return BUI_KEY_M;
        case GLFW_KEY_N:             return BUI_KEY_N;
        case GLFW_KEY_O:             return BUI_KEY_O;
        case GLFW_KEY_P:             return BUI_KEY_P;
        case GLFW_KEY_Q:             return BUI_KEY_Q;
        case GLFW_KEY_R:             return BUI_KEY_R;
        case GLFW_KEY_S:             return BUI_KEY_S;
        case GLFW_KEY_T:             return BUI_KEY_T;
        case GLFW_KEY_U:             return BUI_KEY_U;
        case GLFW_KEY_V:             return BUI_KEY_V;
        case GLFW_KEY_W:             return BUI_KEY_W;
        case GLFW_KEY_X:             return BUI_KEY_X;
        case GLFW_KEY_Y:             return BUI_KEY_Y;
        case GLFW_KEY_Z:             return BUI_KEY_Z;
        case GLFW_KEY_LEFT_BRACKET:  return BUI_KEY_LEFT_BRACKET;
        case GLFW_KEY_BACKSLASH:     return BUI_KEY_BACKSLASH;
        case GLFW_KEY_RIGHT_BRACKET: return BUI_KEY_RIGHT_BRACKET;
        case GLFW_KEY_GRAVE_ACCENT:  return BUI_KEY_GRAVE_ACCENT;
        case GLFW_KEY_WORLD_1:       return BUI_KEY_WORLD_1;
        case GLFW_KEY_WORLD_2:       return BUI_KEY_WORLD_2;
        case GLFW_KEY_ESCAPE:        return BUI_KEY_ESCAPE;
        case GLFW_KEY_ENTER:         return BUI_KEY_ENTER;
        case GLFW_KEY_TAB:           return BUI_KEY_TAB;
        case GLFW_KEY_BACKSPACE:     return BUI_KEY_BACKSPACE;
        case GLFW_KEY_INSERT:        return BUI_KEY_INSERT;
        case GLFW_KEY_DELETE:        return BUI_KEY_DELETE;
        case GLFW_KEY_RIGHT:         return BUI_KEY_RIGHT;
        case GLFW_KEY_LEFT:          return BUI_KEY_LEFT;
        case GLFW_KEY_DOWN:          return BUI_KEY_DOWN;
        case GLFW_KEY_UP:            return BUI_KEY_UP;
        case GLFW_KEY_PAGE_UP:       return BUI_KEY_PAGE_UP;
        case GLFW_KEY_PAGE_DOWN:     return BUI_KEY_PAGE_DOWN;
        case GLFW_KEY_HOME:          return BUI_KEY_HOME;
        case GLFW_KEY_END:           return BUI_KEY_END;
        case GLFW_KEY_CAPS_LOCK:     return BUI_KEY_CAPS_LOCK;
        case GLFW_KEY_SCROLL_LOCK:   return BUI_KEY_SCROLL_LOCK;
        case GLFW_KEY_NUM_LOCK:      return BUI_KEY_NUM_LOCK;
        case GLFW_KEY_PRINT_SCREEN:  return BUI_KEY_PRINT_SCREEN;
        case GLFW_KEY_PAUSE:         return BUI_KEY_PAUSE;
        case GLFW_KEY_F1:            return BUI_KEY_F1;
        case GLFW_KEY_F2:            return BUI_KEY_F2;
        case GLFW_KEY_F3:            return BUI_KEY_F3;
        case GLFW_KEY_F4:            return BUI_KEY_F4;
        case GLFW_KEY_F5:            return BUI_KEY_F5;
        case GLFW_KEY_F6:            return BUI_KEY_F6;
        case GLFW_KEY_F7:            return BUI_KEY_F7;
        case GLFW_KEY_F8:            return BUI_KEY_F8;
        case GLFW_KEY_F9:            return BUI_KEY_F9;
        case GLFW_KEY_F10:           return BUI_KEY_F10;
        case GLFW_KEY_F11:           return BUI_KEY_F11;
        case GLFW_KEY_F12:           return BUI_KEY_F12;
        case GLFW_KEY_F13:           return BUI_KEY_F13;
        case GLFW_KEY_F14:           return BUI_KEY_F14;
        case GLFW_KEY_F15:           return BUI_KEY_F15;
        case GLFW_KEY_F16:           return BUI_KEY_F16;
        case GLFW_KEY_F17:           return BUI_KEY_F17;
        case GLFW_KEY_F18:           return BUI_KEY_F18;
        case GLFW_KEY_F19:           return BUI_KEY_F19;
        case GLFW_KEY_F20:           return BUI_KEY_F20;
        case GLFW_KEY_F21:           return BUI_KEY_F21;
        case GLFW_KEY_F22:           return BUI_KEY_F22;
        case GLFW_KEY_F23:           return BUI_KEY_F23;
        case GLFW_KEY_F24:           return BUI_KEY_F24;
        case GLFW_KEY_F25:           return BUI_KEY_F25;
        case GLFW_KEY_KP_0:          return BUI_KEY_KP_0;
        case GLFW_KEY_KP_1:          return BUI_KEY_KP_1;
        case GLFW_KEY_KP_2:          return BUI_KEY_KP_2;
        case GLFW_KEY_KP_3:          return BUI_KEY_KP_3;
        case GLFW_KEY_KP_4:          return BUI_KEY_KP_4;
        case GLFW_KEY_KP_5:          return BUI_KEY_KP_5;
        case GLFW_KEY_KP_6:          return BUI_KEY_KP_6;
        case GLFW_KEY_KP_7:          return BUI_KEY_KP_7;
        case GLFW_KEY_KP_8:          return BUI_KEY_KP_8;
        case GLFW_KEY_KP_9:          return BUI_KEY_KP_9;
        case GLFW_KEY_KP_DECIMAL:    return BUI_KEY_KP_DECIMAL;
        case GLFW_KEY_KP_DIVIDE:     return BUI_KEY_KP_DIVIDE;
        case GLFW_KEY_KP_MULTIPLY:   return BUI_KEY_KP_MULTIPLY;
        case GLFW_KEY_KP_SUBTRACT:   return BUI_KEY_KP_SUBTRACT;
        case GLFW_KEY_KP_ADD:        return BUI_KEY_KP_ADD;
        case GLFW_KEY_KP_ENTER:      return BUI_KEY_KP_ENTER;
        case GLFW_KEY_KP_EQUAL:      return BUI_KEY_KP_EQUAL;
        case GLFW_KEY_LEFT_SHIFT:    return BUI_KEY_LEFT_SHIFT;
        case GLFW_KEY_LEFT_CONTROL:  return BUI_KEY_LEFT_CONTROL;
        case GLFW_KEY_LEFT_ALT:      return BUI_KEY_LEFT_ALT;
        case GLFW_KEY_LEFT_SUPER:    return BUI_KEY_LEFT_SUPER;
        case GLFW_KEY_RIGHT_SHIFT:   return BUI_KEY_RIGHT_SHIFT;
        case GLFW_KEY_RIGHT_CONTROL: return BUI_KEY_RIGHT_CONTROL;
        case GLFW_KEY_RIGHT_ALT:     return BUI_KEY_RIGHT_ALT;
        case GLFW_KEY_RIGHT_SUPER:   return BUI_KEY_RIGHT_SUPER;
        case GLFW_KEY_MENU:          return BUI_KEY_MENU;
    }

    assert(false);
    return BUI_KEY_UNKNOWN;
}
internal int bui_key_to_glfw(Bui_Key key)
{
    switch(key)
    {
        case BUI_KEY_SPACE:         return GLFW_KEY_SPACE;
        case BUI_KEY_APOSTROPHE:    return GLFW_KEY_APOSTROPHE;
        case BUI_KEY_COMMA:         return GLFW_KEY_COMMA;
        case BUI_KEY_MINUS:         return GLFW_KEY_MINUS;
        case BUI_KEY_PERIOD:        return GLFW_KEY_PERIOD;
        case BUI_KEY_SLASH:         return GLFW_KEY_SLASH;
        case BUI_KEY_0:             return GLFW_KEY_0;
        case BUI_KEY_1:             return GLFW_KEY_1;
        case BUI_KEY_2:             return GLFW_KEY_2;
        case BUI_KEY_3:             return GLFW_KEY_3;
        case BUI_KEY_4:             return GLFW_KEY_4;
        case BUI_KEY_5:             return GLFW_KEY_5;
        case BUI_KEY_6:             return GLFW_KEY_6;
        case BUI_KEY_7:             return GLFW_KEY_7;
        case BUI_KEY_8:             return GLFW_KEY_8;
        case BUI_KEY_9:             return GLFW_KEY_9;
        case BUI_KEY_SEMICOLON:     return GLFW_KEY_SEMICOLON;
        case BUI_KEY_EQUAL:         return GLFW_KEY_EQUAL;
        case BUI_KEY_A:             return GLFW_KEY_A;
        case BUI_KEY_B:             return GLFW_KEY_B;
        case BUI_KEY_C:             return GLFW_KEY_C;
        case BUI_KEY_D:             return GLFW_KEY_D;
        case BUI_KEY_E:             return GLFW_KEY_E;
        case BUI_KEY_F:             return GLFW_KEY_F;
        case BUI_KEY_G:             return GLFW_KEY_G;
        case BUI_KEY_H:             return GLFW_KEY_H;
        case BUI_KEY_I:             return GLFW_KEY_I;
        case BUI_KEY_J:             return GLFW_KEY_J;
        case BUI_KEY_K:             return GLFW_KEY_K;
        case BUI_KEY_L:             return GLFW_KEY_L;
        case BUI_KEY_M:             return GLFW_KEY_M;
        case BUI_KEY_N:             return GLFW_KEY_N;
        case BUI_KEY_O:             return GLFW_KEY_O;
        case BUI_KEY_P:             return GLFW_KEY_P;
        case BUI_KEY_Q:             return GLFW_KEY_Q;
        case BUI_KEY_R:             return GLFW_KEY_R;
        case BUI_KEY_S:             return GLFW_KEY_S;
        case BUI_KEY_T:             return GLFW_KEY_T;
        case BUI_KEY_U:             return GLFW_KEY_U;
        case BUI_KEY_V:             return GLFW_KEY_V;
        case BUI_KEY_W:             return GLFW_KEY_W;
        case BUI_KEY_X:             return GLFW_KEY_X;
        case BUI_KEY_Y:             return GLFW_KEY_Y;
        case BUI_KEY_Z:             return GLFW_KEY_Z;
        case BUI_KEY_LEFT_BRACKET:  return GLFW_KEY_LEFT_BRACKET;
        case BUI_KEY_BACKSLASH:     return GLFW_KEY_BACKSLASH;
        case BUI_KEY_RIGHT_BRACKET: return GLFW_KEY_RIGHT_BRACKET;
        case BUI_KEY_GRAVE_ACCENT:  return GLFW_KEY_GRAVE_ACCENT;
        case BUI_KEY_WORLD_1:       return GLFW_KEY_WORLD_1;
        case BUI_KEY_WORLD_2:       return GLFW_KEY_WORLD_2;
        case BUI_KEY_ESCAPE:        return GLFW_KEY_ESCAPE;
        case BUI_KEY_ENTER:         return GLFW_KEY_ENTER;
        case BUI_KEY_TAB:           return GLFW_KEY_TAB;
        case BUI_KEY_BACKSPACE:     return GLFW_KEY_BACKSPACE;
        case BUI_KEY_INSERT:        return GLFW_KEY_INSERT;
        case BUI_KEY_DELETE:        return GLFW_KEY_DELETE;
        case BUI_KEY_RIGHT:         return GLFW_KEY_RIGHT;
        case BUI_KEY_LEFT:          return GLFW_KEY_LEFT;
        case BUI_KEY_DOWN:          return GLFW_KEY_DOWN;
        case BUI_KEY_UP:            return GLFW_KEY_UP;
        case BUI_KEY_PAGE_UP:       return GLFW_KEY_PAGE_UP;
        case BUI_KEY_PAGE_DOWN:     return GLFW_KEY_PAGE_DOWN;
        case BUI_KEY_HOME:          return GLFW_KEY_HOME;
        case BUI_KEY_END:           return GLFW_KEY_END;
        case BUI_KEY_CAPS_LOCK:     return GLFW_KEY_CAPS_LOCK;
        case BUI_KEY_SCROLL_LOCK:   return GLFW_KEY_SCROLL_LOCK;
        case BUI_KEY_NUM_LOCK:      return GLFW_KEY_NUM_LOCK;
        case BUI_KEY_PRINT_SCREEN:  return GLFW_KEY_PRINT_SCREEN;
        case BUI_KEY_PAUSE:         return GLFW_KEY_PAUSE;
        case BUI_KEY_F1:            return GLFW_KEY_F1;
        case BUI_KEY_F2:            return GLFW_KEY_F2;
        case BUI_KEY_F3:            return GLFW_KEY_F3;
        case BUI_KEY_F4:            return GLFW_KEY_F4;
        case BUI_KEY_F5:            return GLFW_KEY_F5;
        case BUI_KEY_F6:            return GLFW_KEY_F6;
        case BUI_KEY_F7:            return GLFW_KEY_F7;
        case BUI_KEY_F8:            return GLFW_KEY_F8;
        case BUI_KEY_F9:            return GLFW_KEY_F9;
        case BUI_KEY_F10:           return GLFW_KEY_F10;
        case BUI_KEY_F11:           return GLFW_KEY_F11;
        case BUI_KEY_F12:           return GLFW_KEY_F12;
        case BUI_KEY_F13:           return GLFW_KEY_F13;
        case BUI_KEY_F14:           return GLFW_KEY_F14;
        case BUI_KEY_F15:           return GLFW_KEY_F15;
        case BUI_KEY_F16:           return GLFW_KEY_F16;
        case BUI_KEY_F17:           return GLFW_KEY_F17;
        case BUI_KEY_F18:           return GLFW_KEY_F18;
        case BUI_KEY_F19:           return GLFW_KEY_F19;
        case BUI_KEY_F20:           return GLFW_KEY_F20;
        case BUI_KEY_F21:           return GLFW_KEY_F21;
        case BUI_KEY_F22:           return GLFW_KEY_F22;
        case BUI_KEY_F23:           return GLFW_KEY_F23;
        case BUI_KEY_F24:           return GLFW_KEY_F24;
        case BUI_KEY_F25:           return GLFW_KEY_F25;
        case BUI_KEY_KP_0:          return GLFW_KEY_KP_0;
        case BUI_KEY_KP_1:          return GLFW_KEY_KP_1;
        case BUI_KEY_KP_2:          return GLFW_KEY_KP_2;
        case BUI_KEY_KP_3:          return GLFW_KEY_KP_3;
        case BUI_KEY_KP_4:          return GLFW_KEY_KP_4;
        case BUI_KEY_KP_5:          return GLFW_KEY_KP_5;
        case BUI_KEY_KP_6:          return GLFW_KEY_KP_6;
        case BUI_KEY_KP_7:          return GLFW_KEY_KP_7;
        case BUI_KEY_KP_8:          return GLFW_KEY_KP_8;
        case BUI_KEY_KP_9:          return GLFW_KEY_KP_9;
        case BUI_KEY_KP_DECIMAL:    return GLFW_KEY_KP_DECIMAL;
        case BUI_KEY_KP_DIVIDE:     return GLFW_KEY_KP_DIVIDE;
        case BUI_KEY_KP_MULTIPLY:   return GLFW_KEY_KP_MULTIPLY;
        case BUI_KEY_KP_SUBTRACT:   return GLFW_KEY_KP_SUBTRACT;
        case BUI_KEY_KP_ADD:        return GLFW_KEY_KP_ADD;
        case BUI_KEY_KP_ENTER:      return GLFW_KEY_KP_ENTER;
        case BUI_KEY_KP_EQUAL:      return GLFW_KEY_KP_EQUAL;
        case BUI_KEY_LEFT_SHIFT:    return GLFW_KEY_LEFT_SHIFT;
        case BUI_KEY_LEFT_CONTROL:  return GLFW_KEY_LEFT_CONTROL;
        case BUI_KEY_LEFT_ALT:      return GLFW_KEY_LEFT_ALT;
        case BUI_KEY_LEFT_SUPER:    return GLFW_KEY_LEFT_SUPER;
        case BUI_KEY_RIGHT_SHIFT:   return GLFW_KEY_RIGHT_SHIFT;
        case BUI_KEY_RIGHT_CONTROL: return GLFW_KEY_RIGHT_CONTROL;
        case BUI_KEY_RIGHT_ALT:     return GLFW_KEY_RIGHT_ALT;
        case BUI_KEY_RIGHT_SUPER:   return GLFW_KEY_RIGHT_SUPER;
        case BUI_KEY_MENU:          return GLFW_KEY_MENU;
    }

    assert(false);
    return 0;
}

internal int bui_mouse_to_glfw(Bui_Mouse button)
{
    switch(button)
    {
        case BUI_MOUSE_LEFT: return GLFW_MOUSE_BUTTON_LEFT;
    }

    assert(false);
    return 0;
}

internal bool platform_input_is_key_down(Bui_Key key)
{
    return glfwGetKey(g_Window, bui_key_to_glfw(key)) == GLFW_PRESS;
}

internal void platform_input_get_mouse_pos(double* mouse_x, double* mouse_y)
{
    glfwGetCursorPos(g_Window, mouse_x, mouse_y);
    *mouse_y = g_Height - *mouse_y;
}

internal bool platform_input_is_mouse_down(Bui_Mouse button)
{
    return glfwGetMouseButton(g_Window, bui_mouse_to_glfw(button)) == GLFW_PRESS;
}

internal void glfw_error_callback(int error, const char* desc)
{
    fprintf(stderr, "[ERROR]: (%d) %s\n", error, desc);
}

internal void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (action == GLFW_PRESS)
    {
        BuiKeyMods bui_mods = {0};

        if (mods & GLFW_MOD_CONTROL) bui_mods.control = true;
        if (mods & GLFW_MOD_SHIFT)   bui_mods.shift   = true;
        if (mods & GLFW_MOD_ALT)     bui_mods.alt     = true;

        buoyantui_key_pressed_callback(glfw_key_to_bui(key), bui_mods);
    }
}

// int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nShowCmd)
int main(void)
{
    Arena* arena = arena_create(Megabytes(64));

    // GLFW 
    if (!glfwInit())
    {
        return -1;
    }

    g_Window = glfwCreateWindow(1600, 900, "BuoyantUI", NULL, NULL);

    if (g_Window == NULL)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetErrorCallback(glfw_error_callback);
    glfwSetKeyCallback(g_Window, glfw_key_callback);
    glfwMakeContextCurrent(g_Window);
    gladLoadGL(glfwGetProcAddress);

    bool v_sync = true;
    glfwSwapInterval(v_sync);

    // OpenGL 
    printf("OpenGL Info\n");
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    // TODO: this should be setup by renderer...
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // FT_Library ft;
    // if (FT_Init_FreeType(&ft))
    // {
    //     printf("Could not init freetype\n");
    //     assert(false);
    // }
    //
    // FT_Face face;
    // if (FT_New_Face(ft, "resources/fonts/opensans/OpenSans-Regular.ttf", 0, &face))
    // {
    //     printf("Failed to load font\n");
    //     assert(false);
    // }

    // NOTE: init bui
    Bui* bui = bui_init();

    Renderer2D_Data renderer = renderer2D_init(arena);

    //
    // Load level file
    //

    char* tileset = NULL;
    int map_width = 0;
    int map_height = 0;
    int tile_size = 0;
    int** tiles = NULL;
    StringView sv = sv_read_entire_file(arena, "resources/levels/grass-water.level");

    //
    // NOTE: doing it fixed order to be lazy
    // TODO: improve this in future but works for now
    //
    
    //
    // tileset
    //

    sv_chop_by_delim(&sv, ':');
    if (sv.size > 0)
    {
        StringView full_line = sv_chop_line(&sv);
        StringView line = sv_trim(sv_chop_by_delim(&full_line, '#'));
        StringView lhs = sv_trim(sv_chop_by_delim(&line, '='));
        line = sv_trim(line);
        tileset = ArenaPushArray(arena, char, line.size + 1);
        memcpy(tileset, line.data, line.size);
        printf("tileset = %s\n", tileset);
    }

    // 
    // map_width
    //
    
    sv_chop_by_delim(&sv, ':');
    if (sv.size > 0)
    {
        StringView full_line = sv_chop_line(&sv);
        StringView line = sv_trim(sv_chop_by_delim(&full_line, '#'));
        StringView lhs = sv_trim(sv_chop_by_delim(&line, '='));
        line = sv_trim(line);
        map_width = atoi(line.data); // NOTE: This is potentially bad?
        printf("map_width = %d\n", map_width);
    }

    //
    // map_height
    //

    sv_chop_by_delim(&sv, ':');
    if (sv.size > 0)
    {
        StringView full_line = sv_chop_line(&sv);
        StringView line = sv_trim(sv_chop_by_delim(&full_line, '#'));
        StringView lhs = sv_trim(sv_chop_by_delim(&line, '='));
        line = sv_trim(line);
        map_height = atoi(line.data); // NOTE: This is potentially bad?
        printf("map_height = %d\n", map_height);
    }

    //
    // tile_size
    //

    sv_chop_by_delim(&sv, ':');
    if (sv.size > 0)
    {
        StringView full_line = sv_chop_line(&sv);
        StringView line = sv_trim(sv_chop_by_delim(&full_line, '#'));
        StringView lhs = sv_trim(sv_chop_by_delim(&line, '='));
        line = sv_trim(line);
        tile_size = atoi(line.data); // NOTE: This is potentially bad?
        printf("tile_size = %d\n", tile_size);
    }

    //
    // tiles
    //

    tiles = ArenaPushArray(arena, int*, map_height);
    for (int i = 0; i < map_height; i++)
    {
        tiles[i] = ArenaPushArray(arena, int, map_width);
    }

    int x = 0;
    int y = 0;
    sv_chop_by_delim(&sv, ':');
    if (sv.size > 0)
    {
        StringView full_line = sv_chop_line(&sv);
        StringView line = sv_trim(sv_chop_by_delim(&full_line, '#'));
        StringView lhs = sv_trim(sv_chop_by_delim(&line, '='));
        line = sv_trim(line);
        if (line.data[0] == '{')
        {
            StringView rest = sv_chop_left(line, 1);
            if (rest.size > 0)
            {
                line = sv_trim(sv_chop_by_delim(&rest, '#'));
                while (line.size > 0)
                {
                    StringView s = sv_trim(sv_chop_by_delim(&line, ','));
                    tiles[y][x] = atoi(s.data); // NOTE: This is potentially bad?
                    if (x < map_width - 1)
                    {
                        x++;
                    }
                    else if (y < map_height - 1)
                    {
                        x = 0;
                        y++;
                    }
                }
            }
            StringView block = sv_chop_by_delim(&sv, '}');
            while (block.size > 0)
            {
                full_line = sv_chop_line(&block);
                line = sv_trim(sv_chop_by_delim(&full_line, '#'));
                while (line.size > 0)
                {
                    StringView s = sv_trim(sv_chop_by_delim(&line, ','));
                    tiles[y][x] = atoi(s.data); // NOTE: This is potentially bad?
                    if (x < map_width - 1)
                    {
                        x++;
                    }
                    else if (y < map_height - 1)
                    {
                        x = 0;
                        y++;
                    }
                }
            }
        }

        if (x < map_width - 1 || y < map_height - 1)
        {
            printf("not enough tiles supplied\n");
        }
    }

    Buoyantui* buoyantui = ArenaPushStruct(arena, Buoyantui);
    buoyantui->arena = arena;
    buoyantui->bui = bui;
    buoyantui->renderer = &renderer;
    buoyantui->tilemap = texture_create_from_file(tileset); // What happens if error
    buoyantui->map_width = map_width;
    buoyantui->map_height = map_height;
    buoyantui->tile_size = tile_size;
    buoyantui->tiles = tiles;
    
    float last_time = (float)glfwGetTime();
    while(!glfwWindowShouldClose(g_Window))
    {
        glfwPollEvents();

        float time = (float)glfwGetTime();
        float dt = time - last_time;
        last_time = time;

        // TODO: glfwSetFramebufferSizeCallback
        glfwGetFramebufferSize(g_Window, (int*)&g_Width, (int*)&g_Height);

        renderer_api_clear(0, 0, g_Width, g_Height);

        buoyantui_update(buoyantui, (float)g_Width, (float)g_Height, dt);

        glfwSwapBuffers(g_Window);
    }

    glfwTerminate();

    return 0;
}
