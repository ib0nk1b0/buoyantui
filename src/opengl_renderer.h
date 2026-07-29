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
    GLint         TEMPUniformLoc;  // TODO: Temp
    GLint         TEMPUniformLoc2; // TODO: Temp
    GLint         TEMPUniformLoc3; // TODO: Temp
    GLint         TEMPUniformLoc4; // TODO: Temp
    Vertex_Array  VertexArray;
    Vertex_Buffer VertexBuffer;
    Index_Buffer  IndexBuffer;
    Vertex*       VertexBase;
    Vertex*       VertexPtr;
    uint32_t      MaxTextureSlots;
    Texture2D*    TextureSlots;
    uint32_t      TextureSlotIndex;
} Renderer2D_Data;

static Renderer2D_Data renderer2D_init(Arena* arena);
static void renderer2D_begin_scene(Renderer2D_Data* data, mat4 camera, /* TODO:remove -> */ float width, float height, float time);
static void renderer2D_draw_quad(Renderer2D_Data* data, mat4 transform, vec4 color);
static void renderer2D_draw_textured_quad(Renderer2D_Data* data, Texture2D texture, mat4 transform, vec4 color);
static void renderer2D_draw_textured_qaud_uvs(Renderer2D_Data* data, Texture2D texture, mat4 transform, vec4 color, vec2* uvs);
static void renderer2D_draw_string(Renderer2D_Data* data, Texture2D font, const char* string, mat4 transform, vec4 color);
static void renderer2D_draw_string_sized(Renderer2D_Data* data, Texture2D font, const char* string, size_t string_size, mat4 transform, vec4 color);
static void renderer2D_end_scene(Renderer2D_Data* data);

#endif // OPENGL_RENDERER_H
