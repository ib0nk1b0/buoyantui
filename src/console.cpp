#define MAX_OPENNESS 15.0f

// TODO: globals
global float     openness = 0.0f;
global float     openness_target = 0.0f;
global glm::vec3 console_scale;
global glm::vec3 console_input_scale;
global char*     input_buffer;
global uint32_t  cursor_pos = 0;
global uint32_t  scroll = 0;
global Font      console_font; 
global glm::vec4 background_color;
global glm::vec4 input_background_color;
global glm::vec4 input_text_color;
global glm::vec4 main_text_color;
global glm::vec4 other_text_color;

struct ConsoleLine
{
    char* line;
    bool  is_input;
};

struct ConsoleData
{
    ConsoleLine* console_lines = nullptr;
    uint32_t     num_lines = 0;
    uint32_t     line_index = 0;
    bool         has_wrapped = false;
};

static ConsoleData console_data;

typedef enum
{
    CONSOLE_CLOSE,
    CONSOLE_OPEN,
    CONSOLE_OPEN_BIG,
} Console_State;

#define MAX_CONSOLE_LINES 256
#define MAX_CONSOLE_LINE_LEN 256

internal void console_init(Arena* arena)
{
    console_data = {};
    console_data.console_lines = ArenaPushArray(arena, ConsoleLine, MAX_CONSOLE_LINES); // TODO: magic number
    console_data.num_lines = MAX_CONSOLE_LINES;
    for (int i = 0; i < MAX_CONSOLE_LINES; i++)
    {
        console_data.console_lines[i].line = ArenaPushArray(arena, char, MAX_CONSOLE_LINE_LEN);
    }

    input_buffer = ArenaPushArray(arena, char, MAX_CONSOLE_LINE_LEN);

    background_color = glm::vec4(17.f/255.f, 24.f/255.f, 39.f/255.f, 1.0f);
    input_background_color = glm::vec4(27.f/255.f, 38.f/255.f, 56.f/255.f, 1.0f);
    input_text_color = glm::vec4(255.f/255.f, 198.f/255.f, 109.f/255.f, 1.0f);
    other_text_color = glm::vec4(169.f/255.f, 184.f/255.f, 204.f/255.f, 1.0f);
}

internal void console_set_font(const Font& font)
{
    console_font = font;
}

internal bool console_is_open()
{
    return openness > 0.0f;
}

internal void console_scroll(int amount)
{
    if (console_data.has_wrapped) if ((amount < 0 && scroll == 0) || (amount > 0 && scroll + amount > console_data.num_lines - 1)) return;

    if ((amount < 0 && scroll == 0) || (amount > 0 && scroll + amount > console_data.line_index - 1)) return;

    scroll += amount;
}

internal void console_input(char c)
{
    if (cursor_pos < MAX_CONSOLE_LINE_LEN - 1) // NOTE: doing - 1 for null terminator?
    {
        input_buffer[cursor_pos++] = c;
    }
}

internal float console_calculate_cursor_x()
{
    const auto& fontGeometry = console_font.font_data->FontGeometry;
    const auto& metrics = fontGeometry.getMetrics();
    double x = 0.0;
    double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
    const float spaceGlyphAdvance = (float)fontGeometry.getGlyph(' ')->getAdvance();
    for (uint32_t i = 0; i < cursor_pos; i++)
    {
        char character = input_buffer[i];
        
        if (character == '\r')
            continue;

        if (character == '\n')
            continue;


        if (character == '\t')
        {
            x += 4.0f * (fsScale * spaceGlyphAdvance + console_font.kerning);
            continue;
        }

        float advance = (float)fontGeometry.getGlyph(character)->getAdvance();

        x += fsScale * advance + console_font.kerning;
    }

    return (float)x;
}

internal void console_backspace()
{
    if (cursor_pos > 0)
    {
        cursor_pos -= 1;
    }
}

internal void console_enter()
{
    // TODO: process commands
    if (cursor_pos == 0) return;
    char buf[MAX_CONSOLE_LINE_LEN];
    memcpy(buf, input_buffer, cursor_pos);
    buf[cursor_pos] = 0;

    StringView sv = sv_from_cstr(buf);
    StringView command = sv_chop_by_delim(&sv, ' ');
    console_print(buf);
    console_data.console_lines[console_data.line_index - 1].is_input = true;
    if (sv_contains(command, "clear"))
    {
        // TODO: not sure if I want this?
        // if (sv.size >= 0)
        // {
        //     console_print("clear command takes no arguments");
        // }
        // else
        console_data.line_index = 0;
        console_data.has_wrapped = false;
    }
    else if (sv_contains(command, "add"))
    {
        StringView left = sv_chop_by_delim(&sv, ' ');
        StringView right = sv_chop_by_delim(&sv, ' ');
        if (left.size == 0 || right.size == 0 || sv.size > 0)
        {
            console_print("add takes exactly two arguments");
        }
        else
        {
            int a = atoi(left.data); // NOTE: potentially bad
            int b = atoi(right.data); // NOTE: potentially bad
            
            char buf2[MAX_CONSOLE_LINE_LEN];
            sprintf(buf2, "%d + %d = %d", a, b, a + b);
            console_print(buf2);
        }
    }
    else if (sv_contains(command, "quit"))
    {
        game_should_quit = true;
    }
    else
    {
        char buf2[MAX_CONSOLE_LINE_LEN];
        const char* unknown_command = "Unknown Command: ";

        memcpy(buf2, unknown_command, strlen(unknown_command));
        // NOTE: there seems to be a bug if MAX_CONSOLE_LINE_LEN is less than strlen(unknown_command), however, this should never be the case so who cares
        size_t size = command.size < MAX_CONSOLE_LINE_LEN - (strlen(unknown_command) + 1) ? command.size : MAX_CONSOLE_LINE_LEN - (strlen(unknown_command) + 1);

        memcpy(buf2 + strlen(unknown_command), command.data, size);
        buf2[strlen(unknown_command) + size] = 0;

        console_print(buf2);
    }
    cursor_pos = 0;
    scroll = 0;
}

internal void console_print(const char* format, ...)
{
    if (console_data.console_lines == NULL) return;

    if (console_data.line_index >= console_data.num_lines)
    {
        console_data.line_index = 0;
        console_data.has_wrapped = true;
    }

    char* line = console_data.console_lines[console_data.line_index].line;

    va_list args;
    va_start(args, format);
    vsnprintf(line, MAX_CONSOLE_LINE_LEN, format, args);
    va_end(args);

    console_data.console_lines[console_data.line_index].is_input = false;
    console_data.line_index++;
}

internal void console_update_openness(float dt)
{
    // TODO: rate should take into account how far from target it is
    float rate = 10.0f * dt;

    if (openness < openness_target)
    {
        openness += rate * openness_target;
        if (openness > openness_target)
        {
            openness = openness_target;
        }
    }
    else if (openness_target < openness)
    {
        openness -= rate * MAX_OPENNESS * 0.7f;
        if (openness < 0.0f)
        {
            openness = 0.0f;
        }
    }
}

internal void render_console(Renderer2D_Data* renderer, float dt, float screen_width, float screen_height)
{
    console_update_openness(dt);

    if (openness == 0.0f) return;

    glm::mat4 camera_transform, view, projection, camera_view_projection;

    camera_transform = glm::mat4(1.0f);

    view = glm::inverse(camera_transform);

    // TODO: don't know if I want this projection
    float x = 16.0f;
    float y = 9.0f;
    projection = glm::ortho(-x, x, -y, y, -1.0f, 1.0f);
    camera_view_projection = projection * view;

    renderer_api_disable_depth_test();

    renderer2D_begin_scene(renderer, camera_view_projection);

    // TODO: this makes it disappear when closing because target goes to 0

    //
    // Setup scales and translations
    //
    console_scale = { x * 2.0f, MAX_OPENNESS, 1.0f };
    console_input_scale = { x * 2.0f, 0.7f, 1.0f };

    glm::vec3 console_translation(0.0f, y - openness + console_scale.y / 2 + console_input_scale.y, 0.0f);
    glm::vec3 console_input_translation(0.0f, y - openness - console_input_scale.y / 2 + console_input_scale.y, 0.0f);

    //
    // Console Transform
    //
    glm::mat4 console_transform = glm::translate(glm::mat4(1.0f), console_translation) * glm::scale(glm::mat4(1.0f), console_scale);
    
    //
    // Console Input Transform
    //
    glm::mat4 console_input_transform = glm::translate(glm::mat4(1.0f), console_input_translation) * glm::scale(glm::mat4(1.0f), console_input_scale);

    //
    // Draw background
    //
    renderer2D_draw_quad(renderer, console_transform, background_color);

    //
    // Draw lines
    //

    renderer2D_flush(renderer);
    renderer_api_enable_scissor();
    int screen_x = (int)((console_translation.x + x) * (screen_width / (x * 2.0f)) - screen_width * 0.5f);

    float console_bottom = console_translation.y - console_scale.y * 0.5f;
    int int_console_bottom = (int)((console_bottom + y) * (screen_height / (y * 2.0f)));
    int screen_y = int_console_bottom;

    int screen_w = (int)(console_scale.x + x * (screen_width / (x * 2.0f)));
    renderer_api_scissor(screen_x, screen_y, (int)screen_width, (int)screen_height);

    // TODO: think of cleaner way to do this
    if (!console_data.has_wrapped)
    {
        for (uint32_t i = 0; i < console_data.line_index; i++)
        {
            float line_y = console_translation.y - console_scale.y / 2 + (console_data.line_index - i - scroll) * 0.5f - 0.15f;
            glm::vec3 line_translation = { -x + 0.4f, line_y, 0.0f };
            glm::mat4 line_transform = glm::translate(glm::mat4(1.0f), line_translation) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

            glm::vec4 line_color = console_data.console_lines[i].is_input ? input_text_color : other_text_color;

            renderer2D_draw_string(renderer, console_font, console_data.console_lines[i].line, line_transform, line_color);
        }
    }
    else
    {
        for (uint32_t i = 0; i < console_data.num_lines; i++)
        {
            float line_y = console_translation.y - console_scale.y / 2 + (console_data.num_lines - i - scroll) * 0.5f;
            glm::vec3 line_translation = { -x + 0.5f, line_y, 0.0f };
            glm::mat4 line_transform = glm::translate(glm::mat4(1.0f), line_translation) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

            glm::vec4 line_color = console_data.console_lines[i].is_input ? input_text_color : other_text_color;

            renderer2D_draw_string(renderer, console_font, console_data.console_lines[(console_data.line_index + i) % console_data.num_lines].line, line_transform, line_color);
        }
    }

    renderer2D_flush(renderer);
    renderer_api_disable_scissor();

    // 
    // Draw input
    //

    renderer2D_draw_quad(renderer, console_input_transform, input_background_color);

    // TODO: lots of magic numbers going on
    glm::vec3 input_line_translation = { -x + 0.4f, console_input_translation.y - console_input_scale.y / 2 + 0.15f, 0.0f };
    glm::vec3 cursor_translation = { -x + 0.5f + console_calculate_cursor_x() * 0.5f, console_input_translation.y - console_input_scale.y / 2 + 0.3f, 0.0f };

    if (cursor_translation.x + 0.5f >= x)
    {
        input_line_translation.x -= cursor_translation.x - x + 0.5f;
        cursor_translation.x = x - 0.5f;
    }

    glm::mat4 input_line_transform = glm::translate(glm::mat4(1.0f), input_line_translation) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    glm::mat4 cursor_transform = glm::translate(glm::mat4(1.0f), cursor_translation) * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.4f, 1.0f));

    renderer2D_draw_string_sized(renderer, console_font, input_buffer, cursor_pos, input_line_transform, input_text_color);

    renderer2D_draw_quad(renderer, cursor_transform, input_text_color);

    renderer2D_end_scene(renderer);

    renderer_api_enable_depth_test();
}

internal void open_or_close_console(Console_State extent)
{
    switch (extent)
    {
        case CONSOLE_CLOSE: openness_target = 0.0f; break;
        case CONSOLE_OPEN: openness_target = MAX_OPENNESS / 3.0f; break;
        case CONSOLE_OPEN_BIG: openness_target = MAX_OPENNESS; break;
    }
}
