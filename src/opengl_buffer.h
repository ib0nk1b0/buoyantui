#ifndef OPENGL_BUFFER_H
#define OPENGL_BUFFER_H

typedef struct VertexArray Vertex_Array;
typedef struct VertexBuffer Vertex_Buffer;
typedef struct VertexBufferLayout Vertex_Buffer_Layout;
typedef struct IndexBuffer Index_Buffer;

typedef struct
{
    vec3  pos;
    vec4  color;
    vec2  texture_coords;
    float texture_index;
} Vertex;

static const vec3 quadVertexPositions[4] =
{
    { -0.5f, -0.5f, 0.0f },
    {  0.5f, -0.5f, 0.0f },
    {  0.5f,  0.5f, 0.0f },
    { -0.5f,  0.5f, 0.0f },
};
 
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
    uint32_t         offset;
    uint32_t         size;
    Shader_Data_Type type;
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

static Vertex_Array vertex_array_create();
static void vertex_array_bind(Vertex_Array vertexArray);
// TODO: Implement this
// Need to go through the layout of the vertex buffer and setup all the attributes
// Need to think about how to deal with multiple vertex buffers
static void vertex_array_add_vertex_buffer(Vertex_Array* vertexArray, Vertex_Buffer vertexBuffer);
static void vertex_array_set_index_buffer(Vertex_Array* vertexArray, Index_Buffer indexBuffer);

static Vertex_Buffer vertex_buffer_create(float* vertices, size_t size);
static Vertex_Buffer vertex_buffer_create_empty(size_t size);
static void vertex_buffer_bind(Vertex_Buffer vertexBuffer);
// TODO: Implement this
// Need to decide how layout should be stored etc...
static Vertex_Buffer_Layout vertex_buffer_layout_create(Vertex_Buffer_Layout_Element* elements, uint32_t element_count, uint32_t stride);
static void vertex_buffer_set_layout(Vertex_Buffer* vertexBuffer, Vertex_Buffer_Layout layout);
static void vertex_buffer_bind(Vertex_Buffer vertexBuffer);
static void vertex_buffer_set_data(Vertex_Buffer vertexBuffer, const void* data, size_t size);

static Index_Buffer index_buffer_create(uint32_t* indices, uint32_t count);
static void index_buffer_bind(Index_Buffer indexBuffer);

static Vertex_Array vertex_array_create()
{
    Vertex_Array vertexArray = {0};
    glCheckError(glGenVertexArrays(1, &vertexArray.renderer_id));
    return vertexArray;
}

static void vertex_array_bind(Vertex_Array vertexArray)
{
    glCheckError(glBindVertexArray(vertexArray.renderer_id));
}

static void vertex_array_add_vertex_buffer(Vertex_Array* vertexArray, Vertex_Buffer vertexBuffer)
{
    assert(vertexBuffer.layout.element_count && "Must proivde a layout");

    vertex_array_bind(*vertexArray);
    vertex_buffer_bind(vertexBuffer);

    vertexArray->vertex_buffer = vertexBuffer;
    for (int i = 0; i < vertexBuffer.layout.element_count; i++)
    {
        Vertex_Buffer_Layout_Element element = vertexBuffer.layout.elements[i];
        uint32_t num_components = 0;
        switch (element.type)
        {
            case VERTEX_ATTRIB_FLOAT1: num_components = 1;
            case VERTEX_ATTRIB_FLOAT2: num_components = 2;
            case VERTEX_ATTRIB_FLOAT3: num_components = 3;
            case VERTEX_ATTRIB_FLOAT4: num_components = 4;
            {
                // NOTE: integer variant
                // glEnableVertexAttribArray(index);
                // glVertexAttribIPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), layout.GetStride(), (const void*)element.Offset);
                // glVertexAttribDivisor(index, (int)element.PerInstance);

                // TODO: should use the names for the index???? but names require knowing the shader
                // glCheckError(GLint location = glGetAttribLocation(data.QuadShader.renderer_id, "a_Position"));
                glCheckError(glEnableVertexAttribArray(i));
                // TODO: cater for normalized? currently just defaults to false
                glCheckError(glVertexAttribPointer(i, num_components, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)element.offset));
                // TODO: if instanced rendering
                // glVertexAttribDivisor(index, (int)element.PerInstance);
            } break;
            default:
            {
                assert(false && "Not implemented!");
            } break;
        };
    }
}

// TODO: finish
static void vertex_array_set_index_buffer(Vertex_Array* vertexArray, Index_Buffer indexBuffer)
{
    vertex_array_bind(*vertexArray);
    index_buffer_bind(indexBuffer);
    vertexArray->index_buffer = indexBuffer;
}

static Vertex_Buffer vertex_buffer_create(float* vertices, size_t size)
{
    Vertex_Buffer vertexBuffer = {0};
    glCheckError(glCreateBuffers(1, &vertexBuffer.renderer_id));
    glCheckError(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.renderer_id));
    glCheckError(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
    return vertexBuffer;
}

static Vertex_Buffer vertex_buffer_create_empty(size_t size)
{
    Vertex_Buffer vertexBuffer = {0};
    glCreateBuffers(1, &vertexBuffer.renderer_id);
    glCheckError(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.renderer_id));
    glCheckError(glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW));
    return vertexBuffer;
}

static void vertex_buffer_bind(Vertex_Buffer vertexBuffer)
{
    glCheckError(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.renderer_id));
}

static Vertex_Buffer_Layout vertex_buffer_layout_create(Vertex_Buffer_Layout_Element* elements, uint32_t element_count, uint32_t stride)
{
    Vertex_Buffer_Layout layout = {
        .elements =      elements,
        .element_count = element_count,
        .stride = stride,
    };

    return layout;
}

// TODO: this is kind of pointless if all it does is assign the layout to the vertex buffer to be used later.
// doesn't need to be a function call
static void vertex_buffer_set_layout(Vertex_Buffer* vertexBuffer, Vertex_Buffer_Layout layout)
{
    vertexBuffer->layout = layout;
}

static void vertex_buffer_set_data(Vertex_Buffer vertexBuffer, const void* data, size_t size)
{
    glCheckError(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.renderer_id));
    glCheckError(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}


static Index_Buffer index_buffer_create(uint32_t* indices, uint32_t count)
{
    Index_Buffer indexBuffer = {0};

    glCheckError(glCreateBuffers(1, &indexBuffer.renderer_id));
    glCheckError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.renderer_id));
    GLsizeiptr size = count * sizeof(uint32_t);
    glCheckError(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW));

    indexBuffer.count = count;

    return indexBuffer;
}

static void index_buffer_bind(Index_Buffer indexBuffer)
{
    glCheckError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.renderer_id));
}

#endif // OPENGL_BUFFER_H
