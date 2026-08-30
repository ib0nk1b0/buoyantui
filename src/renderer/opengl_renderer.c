#include "opengl_renderer.h"

#include <direct.h>

internal Renderer2D_Data renderer2D_init(Arena* arena)
{
    Renderer2D_Data data = {0};

    data.NumVertices  = 4;
    data.NumIndices   = 6;
    data.MaxQuads     = 1024;
    data.MaxVertices  = data.MaxQuads * data.NumVertices;
    data.MaxIndices   = data.MaxQuads * data.NumIndices;

    data.QuadShader = shader_compile_from_file("src\\basic_shader.glsl");
    data.LineShader = shader_compile_from_file("src\\line_shader.glsl");

    // NOTE: Quad
    data.QuadVertexArray  = vertex_array_create();
    data.QuadVertexBuffer = vertex_buffer_create_empty(data.MaxVertices * sizeof(QuadVertex));

    uint32_t quad_num_elements = 4;
    Vertex_Buffer_Layout_Element* quad_layout_elements = ArenaPushArray(arena, Vertex_Buffer_Layout_Element, quad_num_elements);
    quad_layout_elements[0] = (Vertex_Buffer_Layout_Element){ .name = "a_Position", .type = VERTEX_ATTRIB_FLOAT3, .offset = offsetof(QuadVertex, pos) };
    quad_layout_elements[1] = (Vertex_Buffer_Layout_Element){ .name = "a_Color",    .type = VERTEX_ATTRIB_FLOAT4, .offset = offsetof(QuadVertex, color) };
    quad_layout_elements[2] = (Vertex_Buffer_Layout_Element){ .name = "a_TexCoord", .type = VERTEX_ATTRIB_FLOAT2, .offset = offsetof(QuadVertex, texture_coords) };
    quad_layout_elements[3] = (Vertex_Buffer_Layout_Element){ .name = "a_TexIndex", .type = VERTEX_ATTRIB_FLOAT1, .offset = offsetof(QuadVertex, texture_index) };

    Vertex_Buffer_Layout quad_layout = vertex_buffer_layout_create(quad_layout_elements, quad_num_elements, sizeof(QuadVertex));
    vertex_buffer_set_layout(&data.QuadVertexBuffer, quad_layout);
    vertex_array_add_vertex_buffer(&data.QuadVertexArray, data.QuadVertexBuffer);

    // NOTE: Text
    data.TextVertexArray  = vertex_array_create();
    data.TextVertexBuffer = vertex_buffer_create_empty(data.MaxVertices * sizeof(QuadVertex));

    uint32_t text_num_elements = 4;
    Vertex_Buffer_Layout_Element* text_layout_elements = ArenaPushArray(arena, Vertex_Buffer_Layout_Element, text_num_elements);
    text_layout_elements[0] = (Vertex_Buffer_Layout_Element){ .name = "a_Position", .type = VERTEX_ATTRIB_FLOAT3, .offset = offsetof(QuadVertex, pos) };
    text_layout_elements[1] = (Vertex_Buffer_Layout_Element){ .name = "a_Color",    .type = VERTEX_ATTRIB_FLOAT4, .offset = offsetof(QuadVertex, color) };
    text_layout_elements[2] = (Vertex_Buffer_Layout_Element){ .name = "a_TexCoord", .type = VERTEX_ATTRIB_FLOAT2, .offset = offsetof(QuadVertex, texture_coords) };
    text_layout_elements[3] = (Vertex_Buffer_Layout_Element){ .name = "a_TexIndex", .type = VERTEX_ATTRIB_FLOAT1, .offset = offsetof(QuadVertex, texture_index) };

    Vertex_Buffer_Layout text_layout = vertex_buffer_layout_create(text_layout_elements, text_num_elements, sizeof(QuadVertex));
    vertex_buffer_set_layout(&data.TextVertexBuffer, text_layout);
    vertex_array_add_vertex_buffer(&data.TextVertexArray, data.TextVertexBuffer);

    // NOTE: Line
    data.LineVertexArray  = vertex_array_create();
    data.LineVertexBuffer = vertex_buffer_create_empty(data.MaxVertices * sizeof(LineVertex));

    uint32_t line_num_elements = 2;
    Vertex_Buffer_Layout_Element* line_layout_elements = ArenaPushArray(arena, Vertex_Buffer_Layout_Element, line_num_elements);
    line_layout_elements[0] = (Vertex_Buffer_Layout_Element){ .name = "a_Position", .type = VERTEX_ATTRIB_FLOAT3, .offset = offsetof(LineVertex, pos) };
    line_layout_elements[1] = (Vertex_Buffer_Layout_Element){ .name = "a_Color",    .type = VERTEX_ATTRIB_FLOAT4, .offset = offsetof(LineVertex, color) };

    Vertex_Buffer_Layout line_layout = vertex_buffer_layout_create(line_layout_elements, line_num_elements, sizeof(LineVertex));
    vertex_buffer_set_layout(&data.LineVertexBuffer, line_layout);
    vertex_array_add_vertex_buffer(&data.LineVertexArray, data.LineVertexBuffer);

    uint32_t* indices = ArenaPushArray(arena, uint32_t, data.MaxIndices);
    for (uint32_t i = 0, offset = 0; i < data.MaxIndices; i += 6, offset += 4)
    {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;

        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
    }

    Index_Buffer index_buffer = index_buffer_create(indices, data.MaxIndices);

    ArenaPopArray(arena, uint32_t, data.MaxIndices);

    vertex_array_set_index_buffer(&data.QuadVertexArray, index_buffer);
    vertex_array_set_index_buffer(&data.TextVertexArray, index_buffer);

    data.QuadVertexBase = ArenaPushArray(arena, QuadVertex, data.MaxVertices);
    data.TextVertexBase = ArenaPushArray(arena, QuadVertex, data.MaxVertices);
    data.LineVertexBase = ArenaPushArray(arena, LineVertex, data.MaxVertices);

    data.QuadVertexPtr = data.QuadVertexBase;
    data.TextVertexPtr = data.TextVertexBase;
    data.LineVertexPtr = data.LineVertexBase;

    // TODO: Clenup
    char** uniform_names = ArenaPushArray(arena, char*, 2);
    char* u_ViewProjection = "u_ViewProjection";
    char* u_Textures = "u_Textures";

    uniform_names[0] = ArenaPushArray(arena, char, strlen(u_ViewProjection)+1);
    strcpy(uniform_names[0], u_ViewProjection);
    uniform_names[1] = ArenaPushArray(arena, char, strlen(u_Textures)+1);
    strcpy(uniform_names[1], u_Textures);

    shader_set_uniform_cache(arena, &data.QuadShader, uniform_names, 2);
    shader_set_uniform_cache(arena, &data.LineShader, uniform_names, 2);

    data.MaxTextureSlots = 32;
    data.TextureSlots = ArenaPushArray(arena, Texture2D, data.MaxTextureSlots);
    data.TextureSlotIndex = 0;
    uint32_t textureData = 0xFFFFFFFF;
    data.TextureSlots[data.TextureSlotIndex++] = texture_create(1, 1, GL_RGBA8, GL_RGBA, &textureData);

    data.TextureSamplers = ArenaPushArray(arena, uint32_t, data.MaxTextureSlots);
    for (uint32_t i = 0; i < data.MaxTextureSlots; i++)
    {
        data.TextureSamplers[i] = i;
    }

    data.QuadVertexPositions = ArenaPushArray(arena, vec3, 4);
    glm_vec3_copy((vec3){ -0.5f, -0.5f, 0.0f }, data.QuadVertexPositions[0]);
    glm_vec3_copy((vec3){  0.5f, -0.5f, 0.0f }, data.QuadVertexPositions[1]);
    glm_vec3_copy((vec3){  0.5f,  0.5f, 0.0f }, data.QuadVertexPositions[2]);
    glm_vec3_copy((vec3){ -0.5f,  0.5f, 0.0f }, data.QuadVertexPositions[3]);

    return data;
}

internal void renderer2D_begin_scene(Renderer2D_Data* data, mat4 camera)
{
    shader_bind(&data->QuadShader);
    shader_upload_uniform_mat4(&data->QuadShader, "u_ViewProjection", camera);

    shader_bind(&data->LineShader);
    shader_upload_uniform_mat4(&data->LineShader, "u_ViewProjection", camera);
}

internal void renderer2D_flush(Renderer2D_Data* data)
{
    if (data->QuadIndexCount)
    {
        shader_bind(&data->QuadShader);

        size_t size = (uint8_t*)data->QuadVertexPtr - (uint8_t*)data->QuadVertexBase;
        vertex_buffer_set_data(data->QuadVertexBuffer, data->QuadVertexBase, size);

        for (uint32_t i = 0; i < data->TextureSlotIndex; i++)
        {
            texture_bind(data->TextureSlots[i], i);
        }

        shader_upload_uniform_int_array(&data->QuadShader, "u_Textures", data->TextureSlotIndex, data->TextureSamplers);

        renderer_api_draw_elements(&data->QuadVertexArray, data->QuadIndexCount);
    }

    if (data->TextIndexCount)
    {
        shader_bind(&data->QuadShader);

        size_t size = (uint8_t*)data->TextVertexPtr - (uint8_t*)data->TextVertexBase;
        vertex_buffer_set_data(data->TextVertexBuffer, data->TextVertexBase, size);

        for (uint32_t i = 0; i < data->TextureSlotIndex; i++)
        {
            texture_bind(data->TextureSlots[i], i);
        }

        shader_upload_uniform_int_array(&data->QuadShader, "u_Textures", data->TextureSlotIndex, data->TextureSamplers);

        renderer_api_draw_elements(&data->TextVertexArray, data->TextIndexCount);
    }

    if (data->LineIndexCount)
    {
        shader_bind(&data->LineShader);

        size_t size = (uint8_t*)data->LineVertexPtr - (uint8_t*)data->LineVertexBase;
        vertex_buffer_set_data(data->LineVertexBuffer, data->LineVertexBase, size);

        renderer_api_set_line_width(1.0f);
        renderer_api_draw_lines(&data->LineVertexArray, data->LineIndexCount);
    }

    data->QuadVertexPtr = data->QuadVertexBase;
    data->QuadIndexCount = 0;

    data->TextVertexPtr = data->TextVertexBase;
    data->TextIndexCount = 0;

    data->LineVertexPtr = data->LineVertexBase;
    data->LineIndexCount = 0;
}

internal void renderer2D_end_scene(Renderer2D_Data* data)
{
    renderer2D_flush(data);
}

internal void renderer2D_draw_quad(Renderer2D_Data* data, mat4 transform, vec4 color)
{
    renderer2D_draw_textured_quad(data, data->TextureSlots[0], transform, color);
}

internal void renderer2D_draw_textured_quad(Renderer2D_Data* data, Texture2D texture, mat4 transform, vec4 color)
{
    vec2 quadTextureCoords[4] = 
    {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f }
    };

    renderer2D_draw_textured_qaud_uvs(data, texture, transform, color, quadTextureCoords);
}

internal void renderer2D_draw_textured_qaud_uvs(Renderer2D_Data* data, Texture2D texture, mat4 transform, vec4 color, vec2* uvs)
{
    // TODO: test for end of batch and then flush and start again
    if (data->QuadIndexCount >= data->MaxIndices)
    {
        renderer2D_flush(data);
    }

    float texture_index = -1.0f;
    for (uint32_t i = 0; i < data->TextureSlotIndex; i++)
    {
        if (data->TextureSlots[i].renderer_id == texture.renderer_id)
        {
            texture_index = (float)i;
            break;
        }
    }

    if (texture_index == -1.0f && data->TextureSlotIndex < data->MaxTextureSlots)
    {
        texture_index = (float)data->TextureSlotIndex;
        data->TextureSlots[data->TextureSlotIndex++] = texture;
    }

    for (uint32_t i = 0; i < data->NumVertices; i++)
    {
        vec3 position;
        glm_mat4_mulv3(transform, (float*)data->QuadVertexPositions[i], 1.0f, position);
        glm_vec3_copy(position, data->QuadVertexPtr->pos);
        glm_vec4_copy(color, data->QuadVertexPtr->color);
        glm_vec2_copy((float*)uvs[i], data->QuadVertexPtr->texture_coords);
        data->QuadVertexPtr->texture_index = texture_index;
        data->QuadVertexPtr++;
    }
    data->QuadIndexCount += 6;
}

internal void renderer2D_draw_rect(Renderer2D_Data* data, mat4 transform, vec4 color)
{
    if (data->LineIndexCount + 8 >= data->MaxIndices)
    {
        renderer2D_flush(data);
    }

    vec3 lines[4];
    for (uint8_t i = 0; i < 4; i++)
    {
        vec3 position;
        glm_mat4_mulv3(transform, (float*)data->QuadVertexPositions[i], 1.0f, position);
        glm_vec3_copy(position, lines[i]);
    }

    renderer2D_draw_line(data, (vec2){ lines[0][0], lines[0][1] }, (vec2){ lines[1][0], lines[1][1] }, color);
    renderer2D_draw_line(data, (vec2){ lines[1][0], lines[1][1] }, (vec2){ lines[2][0], lines[2][1] }, color);
    renderer2D_draw_line(data, (vec2){ lines[2][0], lines[2][1] }, (vec2){ lines[3][0], lines[3][1] }, color);
    renderer2D_draw_line(data, (vec2){ lines[3][0], lines[3][1] }, (vec2){ lines[0][0], lines[0][1] }, color);
}

internal void renderer2D_draw_string(Renderer2D_Data* data, Texture2D font, const char* string, mat4 transform, vec4 color)
{
    renderer2D_draw_string_sized(data, font, string, strlen(string), transform, color);
}

internal void renderer2D_draw_string_sized(Renderer2D_Data* data, Texture2D font, const char* string, size_t string_size, mat4 transform, vec4 color)
{
    // TODO: flushing
    // TODO: use different batch pool for strings
    if (data->TextIndexCount + (string_size * 6) >= data->MaxIndices)
    {
        renderer2D_flush(data);
    }

    float texture_index = -1.0f;
    for (uint32_t i = 0; i < data->TextureSlotIndex; i++)
    {
        if (data->TextureSlots[i].renderer_id == font.renderer_id)
        {
            texture_index = (float)i;
            break;
        }
    }

    if (texture_index == -1.0f && data->TextureSlotIndex < data->MaxTextureSlots)
    {
        texture_index = (float)data->TextureSlotIndex;
        data->TextureSlots[data->TextureSlotIndex++] = font;
    }

    size_t x_offset = 0;
    size_t y_offset = 0;
    for (size_t i = 0; i < string_size; i++)
    {
        char c = string[i];
        if (c == '\n')
        {
            x_offset = 0;
            y_offset += 1;
            continue;
        }

        if ((int)c < 32 || (int)c > 127)
        {
            c = '?';
        }

        const size_t index = c - 32;
        const size_t col = index % FONT_COLS;
        const size_t row = index / FONT_COLS;

        size_t pXLeft = col * FONT_CHAR_WIDTH;
        size_t pXRight = pXLeft + FONT_CHAR_WIDTH;
        size_t pYTop = (FONT_ROWS - row) * FONT_CHAR_HEIGHT + 1;
        size_t pYBottom = pYTop - FONT_CHAR_HEIGHT;

        vec2 minNormalized = { (float)pXLeft / (float)FONT_WIDTH, (float)pYBottom / (float)FONT_HEIGHT };
        vec2 maxNormalized = { (float)pXRight / (float)FONT_WIDTH, (float)pYTop / (float)FONT_HEIGHT };

        const vec2 fontTextureCoords[4] = 
        {
            { minNormalized[0], minNormalized[1] },
            { maxNormalized[0], minNormalized[1] },
            { maxNormalized[0], maxNormalized[1] },
            { minNormalized[0], maxNormalized[1] }
        };

        // TODO: test for end of batch and then flush and start again
        for (uint32_t j = 0; j < data->NumVertices; j++)
        {
            vec3 position;
            glm_vec3_copy((float*)data->QuadVertexPositions[j], position);
            position[0] += x_offset;
            position[1] -= y_offset;
            glm_mat4_mulv3(transform, &position[0], 1.0f, position);
            glm_vec3_copy(position, data->TextVertexPtr->pos);
            glm_vec4_copy(color, data->TextVertexPtr->color);
            glm_vec2_copy((float*)fontTextureCoords[j], data->TextVertexPtr->texture_coords);
            data->TextVertexPtr->texture_index = texture_index;
            data->TextVertexPtr++;
        }

        x_offset += 1;
        data->TextIndexCount += 6;
    }

}

internal void renderer2D_draw_line(Renderer2D_Data* data, vec2 p0, vec2 p1, vec4 color)
{
    if (data->LineIndexCount + 2 >= data->MaxIndices)
    {
        renderer2D_flush(data);
    }

    vec3 p0_vec3 = { p0[0], p0[1], 0.0f };
    vec3 p1_vec3 = { p1[0], p1[1], 0.0f };

    glm_vec3_copy(p0_vec3, data->LineVertexPtr->pos);
    glm_vec4_copy(color, data->LineVertexPtr->color);
    data->LineVertexPtr++;

    glm_vec3_copy(p1_vec3, data->LineVertexPtr->pos);
    glm_vec4_copy(color, data->LineVertexPtr->color);
    data->LineVertexPtr++;

    data->LineIndexCount += 2;
}
