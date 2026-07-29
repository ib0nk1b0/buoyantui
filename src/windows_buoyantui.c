#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <Windows.h>

#define CGLM_IMPLEMENTATION
#define CGLM_ALL_UNALIGNED
#include <cglm/cglm.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#ifdef USING_FREE_TYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#endif // USING_FREE_TYPE

// My includes
#include "utils.h"

#include "renderer/opengl_buffer.c"
#include "renderer/opengl_shader.c"
#include "renderer/opengl_texture.c"
#include "renderer/opengl_renderer.c"

#include "bui.c"

static void glfw_error_callback(int error, const char* desc)
{
    fprintf(stderr, "[ERROR]: (%d) %s\n", error, desc);
}

static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

typedef struct
{
    vec3 position;
    vec3 rotation;
    vec3 scale;
} Transform;

int main(void)
{
    srand(time(0));
    Arena* arena = arena_create(Megabytes(64));

    // GLFW 
    GLFWwindow* window = NULL;
    
    if (!glfwInit())
    {
        return -1;
    }

    window = glfwCreateWindow(800, 800, "BuoyantUI", NULL, NULL);

    if (window == NULL)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetErrorCallback(glfw_error_callback);
    glfwSetKeyCallback(window, glfw_key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    // FreeType
#ifdef USING_FREE_TYPE
    FT_Library ft_lib;
    FT_Face font_opensans;

    int error = FT_Init_FreeType(&ft_lib);
    if (error)
    {
        // TODO:
    }

    error = FT_New_Face(ft_lib, "W:\\buoyant\\resources\\fonts\\opensans\\OpenSans-Regular.ttf", 0, &font_opensans);

    if (error == FT_Err_Unknown_File_Format)
    {
        // TODO:
    }
    else if (error)
    {
        // TODO: some other error
    }
#endif // USING_FREE_TYPE

    // OpenGL 
    printf("OpenGL Info:\n");
    printf("    Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("    Renderer: %s\n", glGetString(GL_RENDERER));
    printf("    Version:  %s\n", glGetString(GL_VERSION));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // NOTE: init bui
    Bui* bui = bui_init();

    // Renderer2D_Data data = renderer2D_init(arena);
    // Texture2D fontAtlas = texture_create_from_file("W:\\buoyant\\resources\\textures\\charmap-oldschool_white.png");

    mat4 view, projection, viewProjection;
    glm_mat4_identity(view);

    float scaleFactor = 2.5f;
    // float scaleFactor = 10.0f;
    mat4 transform1, transform2, transform3;
    glm_mat4_identity(transform1);
    glm_mat4_identity(transform2);
    glm_mat4_identity(transform3);
    vec3 scale = { 5.0f, 5.0f, 1.0f };
    // glm_scale(transform1, scale);

    glm_translate_x(transform2, -2.2f);
    glm_translate_y(transform2, -1.5f);
    scale[0] = 0.15f;
    scale[1] = 0.17f;
    glm_scale(transform2, scale);
    scale[0] = 4.00f;
    scale[1] = 2.50f;
    glm_scale(transform3, scale);

    vec4 color_white = { 1.0f, 1.0f, 1.0f, 1.0f };
    vec4 color_red   = { 0.8f, 0.3f, 0.2f, 1.0f };
    vec4 color_green = { 0.2f, 0.8f, 0.3f, 1.0f };
    vec4 color_blue  = { 0.2f, 0.3f, 0.8f, 1.0f };

    // vec4 color;
    // int x = rand() % 4 + 1;
    // if (x == 1) glm_vec4_copy(color_white, color);
    // if (x == 2) glm_vec4_copy(color_red, color);
    // if (x == 3) glm_vec4_copy(color_green, color);
    // if (x == 4) glm_vec4_copy(color_blue, color);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        // TODO: glfwSetFramebufferSizeCallback
        int width, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = (float)width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        float time = glfwGetTime();

        // Input
        // Check if W key is down
        int w_key_state = glfwGetKey(window, GLFW_KEY_W);
        double mouse_x = 0;
        double mouse_y = 0;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        mouse_y = height - mouse_y;
        bui->mouse_x = mouse_x;
        bui->mouse_y = mouse_y;

        // Camera
        // glm_ortho(-ratio * scaleFactor, ratio * scaleFactor, -1.0f * scaleFactor, 1.0f * scaleFactor, -1.0f, 1.0f, projection);
        // glm_mat4_mul(view, projection, viewProjection);

        bui_begin_frame(bui, width, height);

        bui_button(bui, "Button", (vec2){width*0.5f, height*0.5f}, (vec2){100.0f, 60.0f});

        bui_button(bui, "Click", (vec2){800, 800}, (vec2){100.0f, 60.0f});

        char screenWidth[32];
        sprintf(screenWidth, "WIDTH: %d", width);
        bui_text(bui, screenWidth, (vec2){10.0f, height - 10.0f}, (vec2){10.0f, 10.0f});

        char screenHeight[32];
        sprintf(screenHeight, "HEIGHT: %d", height);
        bui_text(bui, screenHeight, (vec2){10.0f, height - 20.0f}, (vec2){10.0f, 10.0f});

        char mousePosBuf[32];
        sprintf(mousePosBuf, "MOUSE POS: %f, %f", mouse_x, mouse_y);
        bui_text(bui, mousePosBuf, (vec2){10.0f, height - 30.0f}, (vec2){10.0f, 10.0f});

        char WKeyDownBuf[32];
        if (w_key_state == GLFW_PRESS || w_key_state == GLFW_REPEAT)
        {
            sprintf(WKeyDownBuf, "W Key Down: TRUE");
        }
        else
        {
            sprintf(WKeyDownBuf, "W Key Down: FALSE");
        }
        bui_text(bui, WKeyDownBuf, (vec2){10.0f, height - 40.0f}, (vec2){10.0f, 10.0f});

        char hoveredBuf[32];
        sprintf(hoveredBuf, "Hovered: %s", bui->hovered);
        bui_text(bui, hoveredBuf, (vec2){10.0f, height - 50.0f}, (vec2){10.0f, 10.0f});

        char activeBuf[32];
        sprintf(activeBuf, "Active: %s", bui->active);
        bui_text(bui, activeBuf, (vec2){10.0f, height - 60.0f}, (vec2){10.0f, 10.0f});

        bui_end_frame(bui);


        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
