#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

typedef struct
{
    char*               name;
    uint32_t            location;
} Shader_Uniform;

typedef struct
{
    Shader_Uniform* uniform_cache;
    uint32_t        uniform_count;
    uint32_t        renderer_id;
} Shader;

static Shader shader_compile_from_file(const char* filepath);

static void shader_set_uniform_cache(Arena* arena, Shader* shader, char** uniform_names, uint32_t uniform_count);

static void shader_upload_uniform_float(Shader shader, const char* name, float value);
static void shader_upload_uniform_mat4(Shader shader, const char* name,  mat4 value);

static void shader_bind(Shader shader);

#endif // OPENGL_SHADER_H
