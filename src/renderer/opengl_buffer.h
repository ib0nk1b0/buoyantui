#ifndef OPENGL_BUFFER_H
#define OPENGL_BUFFER_H

typedef struct VertexArray Vertex_Array;
typedef struct VertexBuffer Vertex_Buffer;
typedef struct VertexBufferLayout Vertex_Buffer_Layout;
typedef struct IndexBuffer Index_Buffer;

typedef enum ShaderDataType
{
    VERTEX_ATTRIB_FLOAT1,
    VERTEX_ATTRIB_FLOAT2,
    VERTEX_ATTRIB_FLOAT3,
    VERTEX_ATTRIB_FLOAT4,
} Shader_Data_Type;

typedef struct VertexBufferLayoutElement
{
    const char*      name;
    Shader_Data_Type type;
    uint32_t         offset;
    uint32_t         size;
} Vertex_Buffer_Layout_Element;

typedef struct VertexBufferLayout
{
    Vertex_Buffer_Layout_Element* elements;
    uint32_t                      element_count;
    uint32_t                      stride;
} Vertex_Buffer_Layout;

typedef struct VertexBuffer
{
    GLuint renderer_id;
    Vertex_Buffer_Layout layout;
} Vertex_Buffer;

typedef struct IndexBuffer
{
    GLuint   renderer_id;
    uint32_t count;
} Index_Buffer;

typedef struct VertexArray
{
    GLuint        renderer_id;
    Vertex_Buffer vertex_buffer;
    Index_Buffer  index_buffer;
} Vertex_Array;

internal Vertex_Array vertex_array_create();
internal void vertex_array_bind(Vertex_Array vertexArray);
// TODO: Implement this
// Need to go through the layout of the vertex buffer and setup all the attributes
// Need to think about how to deal with multiple vertex buffers
internal void vertex_array_add_vertex_buffer(Vertex_Array* vertexArray, Vertex_Buffer vertexBuffer);
internal void vertex_array_set_index_buffer(Vertex_Array* vertexArray, Index_Buffer indexBuffer);

internal Vertex_Buffer vertex_buffer_create(float* vertices, size_t size);
internal Vertex_Buffer vertex_buffer_create_empty(size_t size);
internal void vertex_buffer_bind(Vertex_Buffer vertexBuffer);
// TODO: Implement this
// Need to decide how layout should be stored etc...
internal Vertex_Buffer_Layout vertex_buffer_layout_create(Vertex_Buffer_Layout_Element* elements, uint32_t element_count, uint32_t stride);
internal void vertex_buffer_set_layout(Vertex_Buffer* vertexBuffer, Vertex_Buffer_Layout layout);
internal void vertex_buffer_bind(Vertex_Buffer vertexBuffer);
internal void vertex_buffer_set_data(Vertex_Buffer vertexBuffer, const void* data, size_t size);

internal Index_Buffer index_buffer_create(uint32_t* indices, uint32_t count);
internal void index_buffer_bind(Index_Buffer indexBuffer);

#endif // OPENGL_BUFFER_H
