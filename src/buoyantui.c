#include "buoyantui.h"

internal void buoyantui_render_ui(Bui* bui, float width, float height)
{
    bui_begin_frame(bui, width, height);

    // TODO: figure out why this works when first but not when last
    char mousePosBuf[256];
    sprintf(mousePosBuf, "MOUSE POS: %0.1f, %0.1f", bui->mouse_x, bui->mouse_y);

    char hoveredBuf[256];
    sprintf(hoveredBuf, "Hovered: %s", bui->hovered);

    char activeBuf[256];
    sprintf(activeBuf, "Active: %s", bui->active);

    char screenDimensionsBuf[256];
    sprintf(screenDimensionsBuf, "SCREEN: %0.1f, %0.1f", width, height);

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

    bui_end_frame(bui);
}

internal void buoyantui_update(Arena* arena, Bui* bui, float width, float height)
{

    bool down = platform_input_is_key_down(BUI_KEY_W);
    if (down)
    {
        printf("key down...\n");
    }

    // NOTE: rendering UI last... I had a reason...
    buoyantui_render_ui(bui, width, height);
}
