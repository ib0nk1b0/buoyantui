#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

typedef struct
{
    GLuint renderer_id;
} Shader;

static Shader shader_compile_from_file(const char* filepath);
static void shader_bind(Shader shader);

// TODO: this expects vertex and fragment shader in one file. in the future
// this should probably handle individual shader components to be customizable
static Shader shader_compile_from_file(const char* filepath)
{
    Arena* tempAlloc = arena_create(Megabytes(16));
    StringView sv = sv_read_entire_file(tempAlloc, filepath);

    const char* vertex = NULL;
    const char* fragment = NULL;
    sv_chop_by_delim(&sv, ':');
    if (sv.size != 0)
    {
        StringView lhs = sv_chop_by_delim(&sv, '\n');

        lhs = sv_trim(lhs);
        char* line = sv_make_cstr(tempAlloc, lhs);
        if (strcmp(line, "type vertex") == 0 && vertex == NULL)
        {
            StringView vert = sv_chop_by_delim(&sv, ':');
            vert = sv_trim(vert);
            vertex = sv_make_cstr(tempAlloc, vert);

            lhs = sv_chop_by_delim(&sv, '\n');
            lhs = sv_trim(lhs);
            line = sv_make_cstr(tempAlloc, lhs);
            if (strcmp(line, "type fragment") == 0 && fragment == NULL)
            {
                sv = sv_trim(sv);
                fragment = sv_make_cstr(tempAlloc, sv);
            }
        }
    }

    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex, NULL);
    glCompileShader(vertexShader);
    {
        GLint success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            printf("[ERROR] VERTEX SHADER - %s\n", infoLog);
        }
    }

    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment, NULL);
    glCompileShader(fragmentShader);
    {
        GLint success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            printf("[ERROR] FRAGMENT SHADER - %s\n", infoLog);
        }
    }
    
    const GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    {
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            printf("Program linking error: %s\n", infoLog);
        }
    }

    Shader shader = {
        .renderer_id = program,
    };

    arena_release(tempAlloc);

    return shader;
}

static void shader_bind(Shader shader)
{
    glUseProgram(shader.renderer_id);
}

#endif // OPENGL_SHADER_H
