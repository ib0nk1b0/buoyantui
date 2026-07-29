#include "opengl_renderer.h"

static Renderer2D_Data renderer2D_init(Arena* arena)
{
    Renderer2D_Data data = {0};

    data.NumVertices  = 4;
    data.NumIndices   = 6;
    data.MaxQuads     = 1024; // TODO: <-
    data.MaxVertices  = data.MaxQuads * data.NumVertices;
    data.MaxIndices   = data.MaxQuads * data.NumIndices;

    // TODO: get rid of absolute paths
    data.QuadShader = shader_compile_from_file("W:\\buoyant\\buoyantui\\src\\basic_shader.glsl");
    // data.QuadShader = shader_compile_from_file("W:\\buoyant\\buoyantui\\src\\fractal_shader.glsl");

    data.VertexArray  = vertex_array_create();
    data.VertexBuffer = vertex_buffer_create_empty(data.MaxVertices * sizeof(Vertex));

    // TODO: Set layout
    uint32_t num_elements = 4;
    Vertex_Buffer_Layout_Element* layout_elements = ArenaPushArray(arena, Vertex_Buffer_Layout_Element, num_elements);
    layout_elements[0] = (Vertex_Buffer_Layout_Element){ .name = "a_Position", .type = VERTEX_ATTRIB_FLOAT3, .offset = offsetof(Vertex, pos) };
    layout_elements[1] = (Vertex_Buffer_Layout_Element){ .name = "a_Color", .type = VERTEX_ATTRIB_FLOAT4, .offset = offsetof(Vertex, color) };
    layout_elements[2] = (Vertex_Buffer_Layout_Element){ .name = "a_TexCoord", .type = VERTEX_ATTRIB_FLOAT2, .offset = offsetof(Vertex, texture_coords) };
    layout_elements[3] = (Vertex_Buffer_Layout_Element){ .name = "a_TexIndex", .type = VERTEX_ATTRIB_FLOAT1, .offset = offsetof(Vertex, texture_index) };
    Vertex_Buffer_Layout layout = vertex_buffer_layout_create(layout_elements, num_elements, sizeof(Vertex));
    vertex_buffer_set_layout(&data.VertexBuffer, layout);
    vertex_array_add_vertex_buffer(&data.VertexArray, data.VertexBuffer);

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
    data.IndexBuffer = index_buffer_create(indices, data.MaxIndices);
    ArenaPopArray(arena, uint32_t, data.MaxIndices);
    vertex_array_set_index_buffer(&data.VertexArray, data.IndexBuffer);

    data.VertexBase = ArenaPushArray(arena, Vertex, data.MaxVertices);
    data.VertexPtr = data.VertexBase;

    // TODO: Clenup
    char** uniform_names = ArenaPushArray(arena, char*, 4);
    char* name1 = "u_ViewProjection";
    char* name2 = "u_Width";
    char* name3 = "u_Height";
    char* name4 = "u_Time";

    uniform_names[0] = ArenaPushArray(arena, char, strlen(name1)+1);
    strcpy(uniform_names[0], name1);

    uniform_names[1] = ArenaPushArray(arena, char, strlen(name2)+1);
    strcpy(uniform_names[1], name2);

    uniform_names[2] = ArenaPushArray(arena, char, strlen(name3)+1);
    strcpy(uniform_names[2], name3);

    uniform_names[3] = ArenaPushArray(arena, char, strlen(name4)+1);
    strcpy(uniform_names[3], name4);

    shader_set_uniform_cache(arena, &data.QuadShader, uniform_names, 4);

    // glCheckError(data.TEMPUniformLoc2 = glGetUniformLocation(data.QuadShader.renderer_id, "u_Width"));
    // glCheckError(data.TEMPUniformLoc3 = glGetUniformLocation(data.QuadShader.renderer_id, "u_Height"));
    // glCheckError(data.TEMPUniformLoc4 = glGetUniformLocation(data.QuadShader.renderer_id, "u_Time"));

    data.MaxTextureSlots = 32;
    data.TextureSlots = ArenaPushArray(arena, Texture2D, data.MaxTextureSlots);
    data.TextureSlotIndex = 0;
    uint32_t textureData = 0xFFFFFFFF;
    data.TextureSlots[data.TextureSlotIndex++] = texture_create(1, 1, GL_RGBA8, GL_RGBA, &textureData);

    return data;
}

static void renderer2D_begin_scene(Renderer2D_Data* data, mat4 camera, /*TODO:remove -> */ float width, float height, float time)
{
    data->VertexPtr = data->VertexBase;
    data->QuadIndexCount = 0;
    shader_bind(data->QuadShader);
    // glCheckError(glUniformMatrix4fv(data->TEMPUniformLoc, 1, GL_FALSE, camera[0]));

    // TODO:
    // shader_upload_uniform(data->QuadShader, "u_ViewProjection")
    // glCheckError(glUniformMatrix4fv(data->QuadShader.uniform_cache[0].location, 1, GL_FALSE, camera[0]));
    shader_upload_uniform_mat4(data->QuadShader, "u_ViewProjection", camera);

    shader_upload_uniform_float(data->QuadShader, "u_Width", width);
    shader_upload_uniform_float(data->QuadShader, "u_Height", height);
    shader_upload_uniform_float(data->QuadShader, "u_Time", time);
    // glCheckError(glUniform1f(data->TEMPUniformLoc2, width));
    // glCheckError(glUniform1f(data->TEMPUniformLoc3, height));
    // glCheckError(glUniform1f(data->TEMPUniformLoc4, time));
}

static void renderer2D_draw_quad(Renderer2D_Data* data, mat4 transform, vec4 color)
{
    renderer2D_draw_textured_quad(data, data->TextureSlots[0], transform, color);
}

static void renderer2D_draw_textured_quad(Renderer2D_Data* data, Texture2D texture, mat4 transform, vec4 color)
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

static void renderer2D_draw_textured_qaud_uvs(Renderer2D_Data* data, Texture2D texture, mat4 transform, vec4 color, vec2* uvs)
{
    // TODO: test for end of batch and then flush and start again
    if (data->QuadIndexCount >= data->MaxIndices)
    {
        printf("ERROR: ran out of quads this batch! Implement flushing\n");
        return;
    }
    float texture_index = -1.0f;
    for (int i = 0; i < data->TextureSlotIndex; i++)
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

    for (int i = 0; i < data->NumVertices; i++)
    {
        vec3 position;
        glm_mat4_mulv3(transform, (float*)quadVertexPositions[i], 1.0f, position);
        glm_vec3_copy(position, data->VertexPtr->pos);
        glm_vec4_copy(color, data->VertexPtr->color);
        glm_vec2_copy((float*)uvs[i], data->VertexPtr->texture_coords);
        data->VertexPtr->texture_index = texture_index;
        data->VertexPtr++;
    }
    data->QuadIndexCount += 6;
}

static void renderer2D_draw_string(Renderer2D_Data* data, Texture2D font, const char* string, mat4 transform, vec4 color)
{
    renderer2D_draw_string_sized(data, font, string, strlen(string), transform, color);
}

static void renderer2D_draw_string_sized(Renderer2D_Data* data, Texture2D font, const char* string, size_t string_size, mat4 transform, vec4 color)
{
    // TODO: flushing
    // TODO: use different batch pool for strings
    if (data->QuadIndexCount + (string_size * 6) >= data->MaxIndices)
    {
        printf("ERROR: ran out of quads this batch! Implement flushing\n");
        return;
    }

    float texture_index = -1.0f;
    for (int i = 0; i < data->TextureSlotIndex; i++)
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
        for (int j = 0; j < data->NumVertices; j++)
        {
            vec3 position;
            glm_vec3_copy((float*)quadVertexPositions[j], position);
            position[0] += x_offset;
            position[1] -= y_offset;
            glm_mat4_mulv3(transform, &position[0], 1.0f, position);
            glm_vec3_copy(position, data->VertexPtr->pos);
            glm_vec4_copy(color, data->VertexPtr->color);
            glm_vec2_copy((float*)fontTextureCoords[j], data->VertexPtr->texture_coords);
            data->VertexPtr->texture_index = texture_index;
            data->VertexPtr++;
        }

        x_offset += 1;
        data->QuadIndexCount += 6;
    }

}

static void renderer2D_end_scene(Renderer2D_Data* data)
{
    size_t size = (uint8_t*)data->VertexPtr - (uint8_t*)data->VertexBase;
    vertex_array_bind(data->VertexArray);
    vertex_buffer_set_data(data->VertexBuffer, data->VertexBase, size);
    uint32_t samplers[data->TextureSlotIndex];
    for (uint32_t i = 0; i < data->TextureSlotIndex; i++)
    {
        texture_bind(data->TextureSlots[i], i);
        samplers[i] = i;
    }

    glCheckError(GLint location = glGetUniformLocation(data->QuadShader.renderer_id, "u_Textures"));
    glCheckError(glUniform1iv(location, data->TextureSlotIndex, (int*)&samplers));

    glCheckError(glCheckError(glDrawElements(GL_TRIANGLES, data->QuadIndexCount, GL_UNSIGNED_INT, NULL)));
}

