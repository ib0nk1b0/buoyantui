#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include "opengl_buffer.h"
#include "opengl_shader.h"

typedef struct
{
    uint32_t      NumVertices;
    uint32_t      NumIndices;
    uint32_t      MaxQuads;
    uint32_t      MaxVertices;
    uint32_t      MaxIndices;

    uint32_t      QuadIndexCount;
    Shader        QuadShader;
    Vertex_Array  QuadVertexArray;
    Vertex_Buffer QuadVertexBuffer;
    Index_Buffer  QuadIndexBuffer;
    Vertex*       QuadVertexBase;
    Vertex*       QuadVertexPtr;

    uint32_t      TextIndexCount;
    Vertex_Array  TextVertexArray;
    Vertex_Buffer TextVertexBuffer;
    Index_Buffer  TextIndexBuffer;
    Vertex*       TextVertexBase;
    Vertex*       TextVertexPtr;

    uint32_t      MaxTextureSlots;
    Texture2D*    TextureSlots;
    uint32_t      TextureSlotIndex;
} Renderer2D_Data;

internal Renderer2D_Data renderer2D_init(Arena* arena);
internal void renderer2D_begin_scene(Renderer2D_Data* data, mat4 camera, /* TODO:remove -> */ float width, float height, float time);
internal void renderer2D_draw_quad(Renderer2D_Data* data, mat4 transform, vec4 color);
internal void renderer2D_draw_textured_quad(Renderer2D_Data* data, Texture2D texture, mat4 transform, vec4 color);
internal void renderer2D_draw_textured_qaud_uvs(Renderer2D_Data* data, Texture2D texture, mat4 transform, vec4 color, vec2* uvs);
internal void renderer2D_draw_string(Renderer2D_Data* data, Texture2D font, const char* string, mat4 transform, vec4 color);
internal void renderer2D_draw_string_sized(Renderer2D_Data* data, Texture2D font, const char* string, size_t string_size, mat4 transform, vec4 color);
internal void renderer2D_end_scene(Renderer2D_Data* data);

#endif // OPENGL_RENDERER_H
