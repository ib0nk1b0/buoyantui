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

int main(void)
{
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
    Bui_Color_Scheme default_scheme = bui->color_scheme;
    Bui_Color_Scheme blue_scheme = (Bui_Color_Scheme){
        .font       = { 0.90f, 0.90f, 0.95f, 1.0f },
        .background = { 0.10f, 0.12f, 0.22f, 1.0f },
        .button     = { 0.20f, 0.25f, 0.85f, 1.0f },
        .hovered    = { 0.25f, 0.60f, 0.25f, 1.0f },
    };

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
        int mouse_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (mouse_state == GLFW_PRESS)
        {
            bui->mouse_down = true;
        }
        else
        {
            bui->mouse_down = false;
        }

        // Camera
        // glm_ortho(-ratio * scaleFactor, ratio * scaleFactor, -1.0f * scaleFactor, 1.0f * scaleFactor, -1.0f, 1.0f, projection);
        // glm_mat4_mul(view, projection, viewProjection);

        bui_begin_frame(bui, width, height);

        // if (bui_button(bui, "Blue Scheme", (vec2){width*0.5f, height*0.5f}, (vec2){300.0f, 100.0f}))
        // {
        //     printf("Blue Scheme was clicked\n");
        //     bui->color_scheme = blue_scheme;
        // }
        //
        // if (bui_button(bui, "Reset", (vec2){width*0.5f, height*0.5f-125.0f}, (vec2){300.0f, 100.0f}))
        // {
        //     printf("Reset was clicked\n");
        //     bui->color_scheme = default_scheme;
        // }

        // TODO: figure out why this works when first but not when last
        char mousePosBuf[256];
        sprintf(mousePosBuf, "MOUSE POS: %f, %f", bui->mouse_x, bui->mouse_y);

        char hoveredBuf[256];
        sprintf(hoveredBuf, "Hovered: %s", bui->hovered);

        char activeBuf[256];
        sprintf(activeBuf, "Active: %s", bui->active);

        char screenDimensionsBuf[256];
        sprintf(screenDimensionsBuf, "SCREEN: %d, %d", width, height);

#ifdef BUI_AUTO_LAYOUT
        bui_text(bui, mousePosBuf);
        bui_same_line(bui);
        bui_text(bui, "|");
        bui_same_line(bui);
        bui_text(bui, screenDimensionsBuf);
        bui_text(bui, hoveredBuf);
        bui_same_line(bui);
        bui_text(bui, "|");
        bui_same_line(bui);
        bui_text(bui, activeBuf);

        bui_text(bui, "");

        bui_text(bui, "Some text 1");
        bui_text(bui, "Some text 2");
        bui_text(bui, "Some text 3");
        bui_text(bui, "Some text 4");
        bui_text(bui, "Some text 5");

        bui_text(bui, "");

        bui_button(bui, "Some Button 1");
        bui_same_line(bui);
        bui_button(bui, "Some Button 2");
        bui_same_line(bui);
        bui_button(bui, "Some Button 3");

        bui_text(bui, "");

        if (bui_button(bui, "Set Blue Color Scheme"))
        {
            printf("Blue Scheme was clicked\n");
            bui->color_scheme = blue_scheme;
        }

        if (bui_button(bui, "Reset Color Scheme"))
        {
            printf("Reset was clicked\n");
            bui->color_scheme = default_scheme;
        }

        // Calculator

        typedef enum
        {
            CALC_NONE,
            CALC_ADD,
            CALC_SUB,
            CALC_MUL,
            CALC_DIV,
            CALC_EQUAL,
        } Calculator_Operation_Kind;
        static Calculator_Operation_Kind operation = CALC_NONE; 
        static Calculator_Operation_Kind last_op = CALC_NONE; 

        static int runningTotal = 0;
        static int input = 0;

        char calculatorScreenBuf[256];
        switch (operation)
        {
            case CALC_NONE:
            case CALC_EQUAL:
            {
                sprintf(calculatorScreenBuf, "%d", runningTotal);
            } break;
            case CALC_ADD:
            case CALC_SUB:
            case CALC_MUL:
            case CALC_DIV:
            {
                sprintf(calculatorScreenBuf, "%d", input);
            } break;
        }

        bui_text(bui, "");
        bui_text(bui, calculatorScreenBuf);
        
        if (bui_button(bui, "7"))
        {
            if (operation == CALC_NONE)
            {
                runningTotal = runningTotal * 10 + 7;
            }
            else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
            {
                input = input * 10 + 7;
            }
        }

        bui_same_line(bui);

        if (bui_button(bui, "8"))
        {
            if (operation == CALC_NONE)
            {
                runningTotal = runningTotal * 10 + 8;
            }
            else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
            {
                input = input * 10 + 8;
            }
        }

        bui_same_line(bui);

        if (bui_button(bui, "9"))
        {
            if (operation == CALC_NONE)
            {
                runningTotal = runningTotal * 10 + 9;
            }
            else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
            {
                input = input * 10 + 9;
            }
        }

        bui_same_line(bui);

        if (bui_button(bui, "+"))
        {
            operation = CALC_ADD;
            input = 0;
        }

        if (bui_button(bui, "4"))
        {
            if (operation == CALC_NONE)
            {
                runningTotal = runningTotal * 10 + 4;
            }
            else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
            {
                input = input * 10 + 4;
            }
        }

        bui_same_line(bui);

        if (bui_button(bui, "5"))
        {
            if (operation == CALC_NONE)
            {
                runningTotal = runningTotal * 10 + 5;
            }
            else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
            {
                input = input * 10 + 5;
            }
        }

        bui_same_line(bui);

        if (bui_button(bui, "6"))
        {
            if (operation == CALC_NONE)
            {
                runningTotal = runningTotal * 10 + 6;
            }
            else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
            {
                input = input * 10 + 6;
            }
        }

        bui_same_line(bui);

        if (bui_button(bui, "-"))
        {
            operation = CALC_SUB;
            input = 0;
        }

        if (bui_button(bui, "1"))
        {
            if (operation == CALC_NONE)
            {
                runningTotal = runningTotal * 10 + 1;
            }
            else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
            {
                input = input * 10 + 1;
            }
        }

        bui_same_line(bui);

        if (bui_button(bui, "2"))
        {
            if (operation == CALC_NONE)
            {
                runningTotal = runningTotal * 10 + 2;
            }
            else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
            {
                input = input * 10 + 2;
            }
        }

        bui_same_line(bui);

        if (bui_button(bui, "3"))
        {
            if (operation == CALC_NONE)
            {
                runningTotal = runningTotal * 10 + 3;
            }
            else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
            {
                input = input * 10 + 3;
            }
        }

        bui_same_line(bui);

        if (bui_button(bui, "*"))
        {
            operation = CALC_MUL;
            input = 0;
        }

        if (bui_button(bui, "C"))
        {
            runningTotal = 0;
            input = 0;
            operation = CALC_NONE;
        }

        bui_same_line(bui);

        if (bui_button(bui, "0"))
        {
            if (operation == CALC_NONE)
            {
                runningTotal = runningTotal * 10 + 0;
            }
            else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
            {
                input = input * 10 + 0;
            }
        }

        bui_same_line(bui);

        if (bui_button(bui, "="))
        {
            if (operation == CALC_EQUAL)
            {
                operation = last_op;
            }

            if (operation == CALC_ADD)
            {
                runningTotal += input;
            }

            if (operation == CALC_SUB)
            {
                runningTotal -= input;
            }

            if (operation == CALC_MUL)
            {
                runningTotal *= input;
            }

            if (operation == CALC_DIV)
            {
                if (runningTotal != 0 && input != 0)
                {
                    runningTotal /= input;
                }
            }

            last_op = operation;
            operation = CALC_EQUAL;
        }

        bui_same_line(bui);

        if (bui_button(bui, "/"))
        {
            operation = CALC_DIV;
            input = 0;
        }

        bui_text(bui, "  ");
        bui_same_line(bui);

        if (bui_button(bui, " <= "))
        {
            if (operation == CALC_NONE || operation == CALC_EQUAL)
            {
                if (runningTotal > 0)
                {
                    runningTotal = runningTotal / 10;
                }
            }
            else if (operation == CALC_ADD || operation == CALC_SUB || operation == CALC_MUL || operation == CALC_DIV)
            {
                if (input > 0)
                {
                    input = input / 10;
                }
            }
        }
        
#else
        bui_text(bui, hoveredBuf, (vec2){10.0f, height - 50.0f}, (vec2){10.0f, 10.0f});
        bui_text(bui, activeBuf, (vec2){10.0f, height - 60.0f}, (vec2){10.0f, 10.0f});
#endif // BUI_AUTO_LAYOUT

        // char screenWidth[32];
        // sprintf(screenWidth, "WIDTH: %d", width);
        // bui_text(bui, screenWidth, (vec2){10.0f, height - 10.0f}, (vec2){10.0f, 10.0f});
        //
        // char screenHeight[32];
        // sprintf(screenHeight, "HEIGHT: %d", height);
        // bui_text(bui, screenHeight, (vec2){10.0f, height - 20.0f}, (vec2){10.0f, 10.0f});
        //
        // char mousePosBuf[32];
        // sprintf(mousePosBuf, "MOUSE POS: %f, %f", mouse_x, mouse_y);
        // bui_text(bui, mousePosBuf, (vec2){10.0f, height - 30.0f}, (vec2){10.0f, 10.0f});
        //
        // char WKeyDownBuf[32];
        // if (w_key_state == GLFW_PRESS || w_key_state == GLFW_REPEAT)
        // {
        //     sprintf(WKeyDownBuf, "W Key Down: TRUE");
        // }
        // else
        // {
        //     sprintf(WKeyDownBuf, "W Key Down: FALSE");
        // }
        // bui_text(bui, WKeyDownBuf, (vec2){10.0f, height - 40.0f}, (vec2){10.0f, 10.0f});

        bui_end_frame(bui);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
