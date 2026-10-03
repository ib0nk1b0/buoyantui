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

internal Shader shader_compile_from_file(const char* filepath);

internal void shader_set_uniform_cache(Arena* arena, Shader* shader, char** uniform_names, uint32_t uniform_count);

internal void shader_upload_uniform_int_array(const Shader* shader, const char* name, int count, int* data);
internal void shader_upload_uniform_float(const Shader* shader, const char* name, float value);
internal void shader_upload_uniform_mat4(const Shader* shader, const char* name, const glm::mat4& value);

internal void shader_bind(const Shader* shader);

#endif // OPENGL_SHADER_H
