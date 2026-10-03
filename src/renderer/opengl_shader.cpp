#include "opengl_shader.h"

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
            assert(false);
        }
    }

    Shader shader = {
        .renderer_id = program,
    };

    arena_release(tempAlloc);

    return shader;
}

static void shader_set_uniform_cache(Arena* arena, Shader* shader, char** uniform_names, uint32_t uniform_count)
{
    // TODO: should take in a Shader_Uniform
    // TODO: errors...
    shader->uniform_cache = ArenaPushArray(arena, Shader_Uniform, uniform_count);
    shader->uniform_count = uniform_count;

    for (uint32_t i = 0; i < uniform_count; i++)
    {
        Shader_Uniform* uniform = &shader->uniform_cache[i];
        size_t name_len = strlen(uniform_names[i]);
        uniform->name = ArenaPushArray(arena, char, (name_len + 1));
        
        strcpy(uniform->name, uniform_names[i]);

        glCheckError(uniform->location = glGetUniformLocation(shader->renderer_id, uniform->name));
    }
}

static uint32_t shader_get_location_from_cache(const Shader* shader, const char* name)
{
    uint32_t location = -1;
    for (uint32_t i = 0; i < shader->uniform_count; i++)
    {
        if (strcmp(shader->uniform_cache[i].name, name) == 0)
        {
            location = shader->uniform_cache[i].location;
            break;
        }
    }
    return location;
}

internal void shader_upload_uniform_int_array(const Shader* shader, const char* name, int count, int* data)
{
    uint32_t location = shader_get_location_from_cache(shader, name);
    glCheckError(glUniform1iv(location, count, data));
}

static void shader_upload_uniform_float(const Shader* shader, const char* name, float value)
{
    // NOTE: assuming the cache has already been set up
    // NOTE: check if exists in shader cache...
    uint32_t location = shader_get_location_from_cache(shader, name);
    glCheckError(glUniform1f(location, value));

}

static void shader_upload_uniform_mat4(const Shader* shader, const char* name, const glm::mat4& value)
{
    // NOTE: assuming the cache has already been set up
    // NOTE: check if exists in shader cache...
    uint32_t location = shader_get_location_from_cache(shader, name);
    glCheckError(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
}

static void shader_bind(const Shader* shader)
{
    glUseProgram(shader->renderer_id);
}
