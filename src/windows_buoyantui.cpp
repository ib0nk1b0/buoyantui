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

internal int bui_key_to_glfw(Bui_Key key)
{
    switch(key)
    {
        case BUI_KEY_W: return GLFW_KEY_W;
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

    buoyantui_key_callback(key, scancode, action, mods);
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
    
    while(!glfwWindowShouldClose(g_Window))
    {
        glfwPollEvents();

        // TODO: glfwSetFramebufferSizeCallback
        glfwGetFramebufferSize(g_Window, (int*)&g_Width, (int*)&g_Height);

        renderer_api_clear(0, 0, g_Width, g_Height);

        float time = (float)glfwGetTime();

        buoyantui_update(buoyantui, (float)g_Width, (float)g_Height);

        glfwSwapBuffers(g_Window);
    }

    glfwTerminate();

    return 0;
}
