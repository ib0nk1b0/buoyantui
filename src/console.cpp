#define MAX_OPENNESS 15.0f

// TODO: globals
static float     openness = 0.0f;
static float     openness_target = 0.0f;
static glm::vec3 console_scale;
static glm::vec3 console_input_scale;
static char*     input_buffer;
static uint32_t  cursor_pos = 0;
static uint32_t  scroll = 0;

struct ConsoleLine
{
    char* line;
};

struct ConsoleData
{
    ConsoleLine* console_lines;
    uint32_t     num_lines;
};

static ConsoleData console_data;

typedef enum
{
    CONSOLE_CLOSE,
    CONSOLE_OPEN,
    CONSOLE_OPEN_BIG,
} Console_State;

#define MAX_CONSOLE_LINES 32
#define MAX_CONSOLE_LINE_LEN 256

// TODO: Forward declaration
internal void console_print(const char* text);

internal void console_init(Arena* arena)
{
    console_data = {};
    console_data.console_lines = ArenaPushArray(arena, ConsoleLine, MAX_CONSOLE_LINES); // TODO: magic number
    for (int i = 0; i < MAX_CONSOLE_LINES; i++)
    {
        console_data.console_lines[i].line = ArenaPushArray(arena, char, MAX_CONSOLE_LINE_LEN);
    }

    input_buffer = ArenaPushArray(arena, char, MAX_CONSOLE_LINE_LEN);
}

internal bool console_is_open()
{
    return openness > 0.0f;
}

internal void console_scroll(int amount)
{
    if ((amount < 0 && scroll == 0) || (amount > 0 && scroll + amount > MAX_CONSOLE_LINES)) return;
    scroll += amount;
}

internal void console_input(char c)
{
    if (cursor_pos < MAX_CONSOLE_LINE_LEN - 1) // NOTE: doing - 1 for null terminator?
    {
        input_buffer[cursor_pos++] = c;
    }
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
    if (sv_contains(command, "clear"))
    {
        // TODO: not sure if I want this?
        // if (sv.size >= 0)
        // {
        //     console_print("clear command takes no arguments");
        // }
        // else
        console_data.num_lines = 0;
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
    else
    {
        char buf2[MAX_CONSOLE_LINE_LEN];
        const char* unknown_command = "unknown command: ";
        memcpy(buf2, unknown_command, strlen(unknown_command));
        size_t size = command.size < MAX_CONSOLE_LINE_LEN - strlen(unknown_command) + 1 ? command.size : MAX_CONSOLE_LINE_LEN - strlen(unknown_command) + 1;
        memcpy(buf2 + strlen(unknown_command), command.data, size);
        console_print(buf2);
    }
    cursor_pos = 0;
}

internal void console_print(const char* text)
{
    // TODO: circular buffer
    if (console_data.console_lines == NULL) return; // TODO: error handling

    uint32_t text_len = (uint32_t)strlen(text);
    uint32_t amount_to_copy = text_len < MAX_CONSOLE_LINE_LEN ? text_len : MAX_CONSOLE_LINE_LEN - 1; // NOTE: - 1 for null terminator. Might not need this but will do for now

    if (console_data.num_lines < MAX_CONSOLE_LINES)
    {
        char* line = console_data.console_lines[console_data.num_lines].line;
        memcpy(line, text, amount_to_copy);
        line[amount_to_copy] = 0;
        console_data.num_lines++;
    }
}

internal void console_update_openness(float dt)
{
    // TODO: rate should take into account how far from target it is
    float rate = 40.0f * dt;

    if (openness < openness_target)
    {
        openness += rate;
        if (openness > openness_target)
        {
            openness = openness_target;
        }
    }
    else if (openness_target < openness)
    {
        openness -= rate * 1.5f;
        if (openness < 0.0f)
        {
            openness = 0.0f;
        }
    }
}

internal void render_console(Renderer2D_Data* renderer, Texture2D font_atlas, float dt, float screen_width, float screen_height)
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
    glm::mat4 console_input_transform = glm::translate(glm::mat4(1.0f), console_input_translation)
        * glm::scale(glm::mat4(1.0f), console_input_scale);

    //
    // Draw background
    //
    renderer2D_draw_quad(renderer, console_transform, glm::vec4(0.0f/255.f, 102.f/255.f, 102/255.f, 1.0f));

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
    renderer_api_scissor(screen_x, screen_y, screen_w, (int)screen_height);

    for (uint32_t i = 0; i < console_data.num_lines; i++)
    {
        float line_y = console_translation.y - console_scale.y / 2 + (console_data.num_lines - i - scroll) * 0.5f;
        glm::vec3 line_translation = { -x + 0.5f, line_y, 0.0f };
        glm::mat4 line_transform = glm::translate(glm::mat4(1.0f), line_translation) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
        renderer2D_draw_string(renderer, font_atlas, console_data.console_lines[i].line, line_transform, glm::vec4(1.0f));
    }

    renderer2D_flush(renderer);
    renderer_api_disable_scissor();


    // 
    // Draw input
    //

    renderer2D_draw_quad(renderer, console_input_transform, glm::vec4(0.0f/255.f, 77.f/255.f, 102/255.f, 1.0f));

    glm::vec3 input_line_translation = { -x + 0.5f, console_input_translation.y - console_input_scale.y / 2 + 0.3f, 0.0f };
    glm::mat4 input_line_transform = glm::translate(glm::mat4(1.0f), input_line_translation) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

    renderer2D_draw_string_sized(renderer, font_atlas, input_buffer, cursor_pos, input_line_transform, glm::vec4(1.0f));

    glm::vec3 cursor_translation = { -x + 0.5f + cursor_pos * 0.5f, console_input_translation.y - console_input_scale.y / 2 + 0.3f, 0.0f };
    glm::mat4 cursor_transform = glm::translate(glm::mat4(1.0f), cursor_translation) * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.4f, 1.0f));

    renderer2D_draw_quad(renderer, cursor_transform, glm::vec4(1.0f));

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
