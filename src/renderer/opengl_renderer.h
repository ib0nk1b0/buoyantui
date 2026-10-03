#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include "opengl_buffer.h"
#include "opengl_shader.h"

typedef struct
{
    glm::vec3  pos;
    glm::vec4  color;
    glm::vec2  texture_coords;
    float texture_index;
} QuadVertex;

typedef struct
{
    glm::vec3 pos;
    glm::vec4 color;
} LineVertex;

// TODO: ogranise by size
typedef struct
{
    uint32_t      NumVertices;
    uint32_t      NumIndices;
    uint32_t      MaxQuads;
    uint32_t      MaxVertices;
    uint32_t      MaxIndices;

    Shader        QuadShader;
    Shader        LineShader;

    uint32_t      QuadIndexCount;
    Vertex_Array  QuadVertexArray;
    Vertex_Buffer QuadVertexBuffer;
    QuadVertex*   QuadVertexBase;
    QuadVertex*   QuadVertexPtr;

    uint32_t      TextIndexCount;
    Vertex_Array  TextVertexArray;
    Vertex_Buffer TextVertexBuffer;
    QuadVertex*   TextVertexBase;
    QuadVertex*   TextVertexPtr;

    uint32_t      LineIndexCount;
    Vertex_Array  LineVertexArray;
    Vertex_Buffer LineVertexBuffer;
    LineVertex*   LineVertexBase;
    LineVertex*   LineVertexPtr;

    uint32_t      MaxTextureSlots;
    Texture2D*    TextureSlots;
    uint32_t      TextureSlotIndex;
    uint32_t*     TextureSamplers;

    glm::vec4*    QuadVertexPositions; 
} Renderer2D_Data;

// TODO: handle own Arena?
internal Renderer2D_Data renderer2D_init(Arena* arena);

internal void renderer2D_begin_scene(Renderer2D_Data* data, const glm::mat4& camera);
internal void renderer2D_end_scene(Renderer2D_Data* data);

internal void renderer2D_draw_quad(Renderer2D_Data* data, const glm::mat4& transform, const glm::vec4& color);
internal void renderer2D_draw_textured_quad(Renderer2D_Data* data, Texture2D texture, const glm::mat4& transform, const glm::vec4& color);
internal void renderer2D_draw_textured_qaud_uvs(Renderer2D_Data* data, Texture2D texture, const glm::mat4& transform, const glm::vec4& color, const glm::vec2* uvs); // TODO: look at this

internal void renderer2D_draw_rect(Renderer2D_Data* data, const glm::mat4& transform, const glm::vec4& color);

internal void renderer2D_draw_string(Renderer2D_Data* data, Texture2D font, const char* string, const glm::mat4& transform, const glm::vec4& color);
internal void renderer2D_draw_string_sized(Renderer2D_Data* data, Texture2D font, const char* string, size_t string_size, const glm::mat4& transform, const glm::vec4& color);

internal void renderer2D_draw_line(Renderer2D_Data* data, const glm::vec2& p0, const glm::vec2& p1, const glm::vec4& color);

#endif // OPENGL_RENDERER_H
