#include "buoyantui.h"
#include "renderer/opengl_renderer.h"

static bool show_menu = false;
static bool show_grid = false;

internal void test_ui_stuff(Bui* bui, float width, float height)
{
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

    // TODO: figure out how to handle the auto layout when different items are supposed to move the cursor down by different amounts
    static bool checked = false;
    bui_checkbox(bui, "Checkbox", &checked);

    static bool checked2 = false;
    bui_checkbox(bui, "Checkbox2", &checked2);
    // bui_same_line(bui);
    bui_button(bui, "btn"); // also doesn't work properly

    static bool checked3 = false;
    bui_checkbox(bui, "##Checkbox3", &checked3);
    bui_same_line(bui);
    bui_text(bui, "Checkbox3"); // Creates an interesting bug as text takes up less vertical space so next thing to be rendered gets rendered on top of checkbox

    // TODO: bring in the setup code that runs before the loop somehow
    // if (bui_button(bui, "Set Blue Color Scheme"))
    // {
    //     printf("Blue Scheme was clicked\n");
    //     bui->color_scheme = blue_scheme;
    // }
    //
    // if (bui_button(bui, "Reset Color Scheme"))
    // {
    //     printf("Reset was clicked\n");
    //     bui->color_scheme = default_scheme;
    // }

    bui_begin_window(bui, "A Window", (int)(width*0.5f), (int)(height*0.5f), 200, 200);

    bui_text(bui, "[1]Some Text");
    bui_text(bui, "[2]Some Text");
    bui_text(bui, "[3]Some Text");
    bui_text(bui, "[4]Some Text");
    bui_text(bui, "[5]Some Text");
    bui_text(bui, "[6]Some Text");

    bui_end_window(bui);

    static bool show_calculator = true;
    if (bui_button(bui, "Toggle Calculator"))
    {
        show_calculator = !show_calculator;
    }
    if (show_calculator)
    {
        render_calculator(bui);
    }
}

internal void stats_menu(Bui* bui, float width, float height)
{
    if (show_menu)
    {
        // TODO: figure out why this works when first but not when last
        char mousePosBuf[256];
        sprintf(mousePosBuf, "MOUSE POS: %0.1f, %0.1f", bui->mouse_x, bui->mouse_y);

        char hoveredBuf[256];
        sprintf(hoveredBuf, "Hovered: %s", bui->hovered);

        char activeBuf[256];
        sprintf(activeBuf, "Active: %s", bui->active);

        char screenDimensionsBuf[256];
        sprintf(screenDimensionsBuf, "SCREEN: %0.1f, %0.1f", width, height);

        char fontSizeBuf[64];
        sprintf(fontSizeBuf, "Font Size: %0.1f", bui->font_size);
        char itemPaddingBuf[64];
        sprintf(itemPaddingBuf, "Item Padding: %0.1f", bui->item_padding);
        char buttonPaddingBuf[64];
        sprintf(buttonPaddingBuf, "Button Padding: %0.1f", bui->button_padding);

        int state_width = 300;
        int state_height = 300;
        int state_x = (int)width - state_width;
        int state_y = (int)height - state_height;

        bui_begin_window(bui, "State", state_x, state_y, state_width, state_height);

        bui_text(bui, mousePosBuf);
        bui_text(bui, screenDimensionsBuf);
        bui_text(bui, hoveredBuf);
        bui_text(bui, activeBuf);
        bui_text(bui, fontSizeBuf);
        bui_text(bui, itemPaddingBuf);
        bui_text(bui, buttonPaddingBuf);

        bui_end_window(bui);
    }
}

internal void buoyantui_render_ui(Bui* bui, float width, float height)
{
    bui_begin_frame(bui, width, height);

    // test_ui_stuff(bui, width, height);

    stats_menu(bui, width, height);

    bui_end_frame(bui);
}

internal void render_tile_map(Buoyantui* buoyantui)
{
    for (int i = 0; i < buoyantui->map_height; i++)
    {
        for (int j = 0; j < buoyantui->map_width; j++)
        {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(j - buoyantui->map_width / 2, i - buoyantui->map_height / 2, 0.0f))
                * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

            int tile = buoyantui->tiles[i][j];
            float tile_w = (float)buoyantui->tilemap.width / (float)buoyantui->tile_size;
            float tile_h = (float)buoyantui->tilemap.height / (float)buoyantui->tile_size;

            float x0 = (float)(tile % (int)tile_w);
            if (x0 > 0.0f)
            {
                x0 /= tile_w;
            }

            float y0 = std::floor(tile / tile_w);
            if (y0 > 0.0f)
            {
                 y0 /= tile_h;
            }

            float x1 = x0 + 1.0f / tile_w;
            float y1 = y0 + 1.0f / tile_h;

            glm::vec2 uvs[4] = {
                { x0, y0 },
                { x1, y0 },
                { x1, y1 },
                { x0, y1 }
            };

            renderer2D_draw_textured_quad_uvs(buoyantui->renderer, buoyantui->tilemap, transform, glm::vec4(1.0f), uvs);

            if (show_grid)
            {
                transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.5f));
                renderer2D_draw_rect(buoyantui->renderer, transform, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
            }
        }
    }
}

internal void buoyantui_update(Buoyantui* buoyantui, float width, float height, float dt)
{
    static glm::vec3 position = {0.0f, 0.0f, 0.3f };

    glm::mat4 camera, view, projection, viewProjection;

    camera = glm::mat4(1.0f);

    view = glm::inverse(camera);

    projection = glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f);
    viewProjection = projection * view;

    renderer2D_begin_scene(buoyantui->renderer, viewProjection);

    render_tile_map(buoyantui);

    float speed = 5.0f;
    if (platform_input_is_key_down(BUI_KEY_W))
    {
        position.y += speed * dt;
    }
    if (platform_input_is_key_down(BUI_KEY_S))
    {
        position.y -= speed * dt;
    }
    if (platform_input_is_key_down(BUI_KEY_D))
    {
        position.x += speed * dt;
    }
    if (platform_input_is_key_down(BUI_KEY_A))
    {
        position.x -= speed * dt;
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

    renderer2D_draw_quad(buoyantui->renderer, transform, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    renderer2D_end_scene(buoyantui->renderer);

    // NOTE: rendering UI last... I had a reason...
    buoyantui_render_ui(buoyantui->bui, width, height);
}

internal void buoyantui_key_callback(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
    {
        show_menu = !show_menu;
    }

    if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
        show_grid = !show_grid;
    }
}
