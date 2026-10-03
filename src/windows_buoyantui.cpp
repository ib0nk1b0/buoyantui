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
    // TODO: this setup stuff needs to be available in platform-independant layer
    // Bui_Color_Scheme default_scheme = bui->color_scheme;
    // Bui_Color_Scheme blue_scheme = (Bui_Color_Scheme){
    //     .font       = { 0.90f, 0.90f, 0.95f, 1.0f },
    //     .background = { 0.10f, 0.12f, 0.22f, 1.0f },
    //     .button     = { 0.20f, 0.25f, 0.85f, 1.0f },
    //     .hovered    = { 0.25f, 0.60f, 0.25f, 1.0f },
    // };

    while(!glfwWindowShouldClose(g_Window))
    {
        glfwPollEvents();

        // TODO: glfwSetFramebufferSizeCallback
        glfwGetFramebufferSize(g_Window, (int*)&g_Width, (int*)&g_Height);

        renderer_api_clear(0, 0, g_Width, g_Height);

        float time = (float)glfwGetTime();

        buoyantui_update(arena, bui, (float)g_Width, (float)g_Height);

        glfwSwapBuffers(g_Window);
    }

    glfwTerminate();

    return 0;
}
