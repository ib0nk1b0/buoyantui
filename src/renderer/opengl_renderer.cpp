#include "opengl_renderer.h"

#include <direct.h>

internal Renderer2D_Data renderer2D_init(Arena* arena)
{
    renderer_api_enable_depth_test();
    renderer_api_enable_blend();

    Renderer2D_Data data = {0};

    data.NumVertices  = 4;
    data.NumIndices   = 6;
    data.MaxQuads     = 1024;
    data.MaxVertices  = data.MaxQuads * data.NumVertices;
    data.MaxIndices   = data.MaxQuads * data.NumIndices;

    data.QuadShader  = shader_compile_from_file("resources\\shaders\\basic_shader.glsl");
    data.TextShader = shader_compile_from_file("resources\\shaders\\text_shader.glsl");
    data.LineShader  = shader_compile_from_file("resources\\shaders\\line_shader.glsl");

    // NOTE: Quad
    data.QuadVertexArray  = vertex_array_create();
    data.QuadVertexBuffer = vertex_buffer_create_empty(data.MaxVertices * sizeof(QuadVertex));

    uint32_t quad_num_elements = 4;
    Vertex_Buffer_Layout_Element* quad_layout_elements = ArenaPushArray(arena, Vertex_Buffer_Layout_Element, quad_num_elements);
    quad_layout_elements[0] = { .name = "a_Position", .type = VERTEX_ATTRIB_FLOAT3, .offset = offsetof(QuadVertex, pos) };
    quad_layout_elements[1] = { .name = "a_Color",    .type = VERTEX_ATTRIB_FLOAT4, .offset = offsetof(QuadVertex, color) };
    quad_layout_elements[2] = { .name = "a_TexCoord", .type = VERTEX_ATTRIB_FLOAT2, .offset = offsetof(QuadVertex, texture_coords) };
    quad_layout_elements[3] = { .name = "a_TexIndex", .type = VERTEX_ATTRIB_FLOAT1, .offset = offsetof(QuadVertex, texture_index) };

    Vertex_Buffer_Layout quad_layout = vertex_buffer_layout_create(quad_layout_elements, quad_num_elements, sizeof(QuadVertex));
    vertex_buffer_set_layout(&data.QuadVertexBuffer, quad_layout);
    vertex_array_add_vertex_buffer(&data.QuadVertexArray, data.QuadVertexBuffer);

    // NOTE: Text
    data.TextVertexArray  = vertex_array_create();
    data.TextVertexBuffer = vertex_buffer_create_empty(data.MaxVertices * sizeof(QuadVertex));

    uint32_t text_num_elements = 4;
    Vertex_Buffer_Layout_Element* text_layout_elements = ArenaPushArray(arena, Vertex_Buffer_Layout_Element, text_num_elements);
    text_layout_elements[0] = { .name = "a_Position", .type = VERTEX_ATTRIB_FLOAT3, .offset = offsetof(QuadVertex, pos) };
    text_layout_elements[1] = { .name = "a_Color",    .type = VERTEX_ATTRIB_FLOAT4, .offset = offsetof(QuadVertex, color) };
    text_layout_elements[2] = { .name = "a_TexCoord", .type = VERTEX_ATTRIB_FLOAT2, .offset = offsetof(QuadVertex, texture_coords) };
    text_layout_elements[3] = { .name = "a_TexIndex", .type = VERTEX_ATTRIB_FLOAT1, .offset = offsetof(QuadVertex, texture_index) };

    Vertex_Buffer_Layout text_layout = vertex_buffer_layout_create(text_layout_elements, text_num_elements, sizeof(QuadVertex));
    vertex_buffer_set_layout(&data.TextVertexBuffer, text_layout);
    vertex_array_add_vertex_buffer(&data.TextVertexArray, data.TextVertexBuffer);

    // NOTE: Glyph
    // data.GlyphVertexArray  = vertex_array_create();
    // data.GlyphVertexBuffer = vertex_buffer_create_empty(data.MaxVertices * sizeof(QuadVertex));

    // uint32_t glyph_num_elements = 4;
    // Vertex_Buffer_Layout_Element* glyph_layout_elements = ArenaPushArray(arena, Vertex_Buffer_Layout_Element, glyph_num_elements);
    // glyph_layout_elements[0] = { .name = "a_Position", .type = VERTEX_ATTRIB_FLOAT3, .offset = offsetof(QuadVertex, pos) };
    // glyph_layout_elements[1] = { .name = "a_Color",    .type = VERTEX_ATTRIB_FLOAT4, .offset = offsetof(QuadVertex, color) };
    // glyph_layout_elements[2] = { .name = "a_TexCoord", .type = VERTEX_ATTRIB_FLOAT2, .offset = offsetof(QuadVertex, texture_coords) };
    // glyph_layout_elements[3] = { .name = "a_TexIndex", .type = VERTEX_ATTRIB_FLOAT1, .offset = offsetof(QuadVertex, texture_index) };
    //
    // Vertex_Buffer_Layout glyph_layout = vertex_buffer_layout_create(glyph_layout_elements, glyph_num_elements, sizeof(QuadVertex));
    // vertex_buffer_set_layout(&data.GlyphVertexBuffer, glyph_layout);
    // vertex_array_add_vertex_buffer(&data.GlyphVertexArray, data.GlyphVertexBuffer);

    // NOTE: Line
    data.LineVertexArray  = vertex_array_create();
    data.LineVertexBuffer = vertex_buffer_create_empty(data.MaxVertices * sizeof(LineVertex));

    uint32_t line_num_elements = 2;
    Vertex_Buffer_Layout_Element* line_layout_elements = ArenaPushArray(arena, Vertex_Buffer_Layout_Element, line_num_elements);
    line_layout_elements[0] = { .name = "a_Position", .type = VERTEX_ATTRIB_FLOAT3, .offset = offsetof(LineVertex, pos) };
    line_layout_elements[1] = { .name = "a_Color",    .type = VERTEX_ATTRIB_FLOAT4, .offset = offsetof(LineVertex, color) };

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
    // vertex_array_set_index_buffer(&data.GlyphVertexArray, index_buffer);
    vertex_array_set_index_buffer(&data.TextVertexArray, index_buffer);

    data.QuadVertexBase = ArenaPushArray(arena, QuadVertex, data.MaxVertices);
    data.TextVertexBase = ArenaPushArray(arena, QuadVertex, data.MaxVertices);
    // data.GlyphVertexBase = ArenaPushArray(arena, QuadVertex, data.MaxVertices);
    data.LineVertexBase = ArenaPushArray(arena, LineVertex, data.MaxVertices);

    data.QuadVertexPtr = data.QuadVertexBase;
    data.TextVertexPtr = data.TextVertexBase;
    // data.GlyphVertexPtr = data.GlyphVertexBase;
    data.LineVertexPtr = data.LineVertexBase;

    // TODO: Clenup
    char** uniform_names = ArenaPushArray(arena, char*, 2);
    char u_ViewProjection[] = "u_ViewProjection";
    char u_Textures[] = "u_Textures";

    uniform_names[0] = ArenaPushArray(arena, char, strlen(u_ViewProjection)+1);
    strcpy(uniform_names[0], u_ViewProjection);
    uniform_names[1] = ArenaPushArray(arena, char, strlen(u_Textures)+1);
    strcpy(uniform_names[1], u_Textures);

    shader_set_uniform_cache(arena, &data.QuadShader, uniform_names, 2);
    shader_set_uniform_cache(arena, &data.TextShader, uniform_names, 2);
    shader_set_uniform_cache(arena, &data.LineShader, uniform_names, 2);

    data.MaxTextureSlots = 32;
    data.TextureSlots = ArenaPushArray(arena, Texture2D, data.MaxTextureSlots);
    data.TextureSlotIndex = 0;
    uint32_t textureData = 0xFFFFFFFF;
    data.TextureSlots[data.TextureSlotIndex++] = texture_create(1, 1, GL_RGBA8, GL_RGBA, &textureData, false);

    data.TextureSamplers = ArenaPushArray(arena, uint32_t, data.MaxTextureSlots);
    for (uint32_t i = 0; i < data.MaxTextureSlots; i++)
    {
        data.TextureSamplers[i] = i;
    }

    data.QuadVertexPositions = ArenaPushArray(arena, glm::vec4, 4);
    data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
    data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
    data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

    return data;
}

internal void renderer2D_begin_scene(Renderer2D_Data* data, const glm::mat4& camera)
{
    shader_bind(&data->QuadShader);
    shader_upload_uniform_mat4(&data->QuadShader, "u_ViewProjection", camera);

    shader_bind(&data->TextShader);
    shader_upload_uniform_mat4(&data->TextShader, "u_ViewProjection", camera);

    shader_bind(&data->LineShader);
    shader_upload_uniform_mat4(&data->LineShader, "u_ViewProjection", camera);

    renderer2D_start_batch(data);
}

internal void renderer2D_start_batch(Renderer2D_Data* data)
{
    data->QuadVertexPtr = data->QuadVertexBase;
    data->QuadIndexCount = 0;

    data->TextVertexPtr = data->TextVertexBase;
    data->TextIndexCount = 0;

    // data->GlyphVertexPtr = data->GlyphVertexBase;
    // data->GlyphIndexCount = 0;

    data->LineVertexPtr = data->LineVertexBase;
    data->LineIndexCount = 0;
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

        shader_upload_uniform_int_array(&data->QuadShader, "u_Textures", data->TextureSlotIndex, (int*)data->TextureSamplers);

        renderer_api_draw_elements(&data->QuadVertexArray, data->QuadIndexCount);
    }

    if (data->TextIndexCount)
    {
        shader_bind(&data->TextShader);

        size_t size = (uint8_t*)data->TextVertexPtr - (uint8_t*)data->TextVertexBase;
        vertex_buffer_set_data(data->TextVertexBuffer, data->TextVertexBase, size);

        for (uint32_t i = 0; i < data->TextureSlotIndex; i++)
        {
            texture_bind(data->TextureSlots[i], i);
        }

        shader_upload_uniform_int_array(&data->TextShader, "u_Textures", data->TextureSlotIndex, (int*)data->TextureSamplers);

        renderer_api_draw_elements(&data->TextVertexArray, data->TextIndexCount);
    }

    // if (data->GlyphIndexCount)
    // {
    //     shader_bind(&data->GlyphShader);
    //
    //     size_t size = (uint8_t*)data->GlyphVertexPtr - (uint8_t*)data->GlyphVertexBase;
    //     vertex_buffer_set_data(data->GlyphVertexBuffer, data->GlyphVertexBase, size);
    //
    //     for (uint32_t i = 0; i < data->TextureSlotIndex; i++)
    //     {
    //         texture_bind(data->TextureSlots[i], i);
    //     }
    //
    //     shader_upload_uniform_int_array(&data->GlyphShader, "u_Textures", data->TextureSlotIndex, (int*)data->TextureSamplers);
    //
    //     renderer_api_draw_elements(&data->GlyphVertexArray, data->GlyphIndexCount);
    // }

    if (data->LineIndexCount)
    {
        shader_bind(&data->LineShader);

        size_t size = (uint8_t*)data->LineVertexPtr - (uint8_t*)data->LineVertexBase;
        vertex_buffer_set_data(data->LineVertexBuffer, data->LineVertexBase, size);

        renderer_api_set_line_width(1.0f);
        renderer_api_draw_lines(&data->LineVertexArray, data->LineIndexCount);
    }

    renderer2D_start_batch(data);
}

internal void renderer2D_end_scene(Renderer2D_Data* data)
{
    renderer2D_flush(data);
}

internal void renderer2D_draw_quad(Renderer2D_Data* data, const glm::mat4& transform, const glm::vec4& color)
{
    renderer2D_draw_textured_quad(data, data->TextureSlots[0], transform, color);
}

internal void renderer2D_draw_textured_quad(Renderer2D_Data* data, Texture2D texture, const glm::mat4& transform, const glm::vec4& color)
{
    glm::vec2 quadTextureCoords[4] = 
    {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f }
    };

    renderer2D_draw_textured_quad_uvs(data, texture, transform, color, quadTextureCoords);
}

internal void renderer2D_draw_textured_quad_uvs(Renderer2D_Data* data, Texture2D texture, const glm::mat4& transform, const glm::vec4& color, const glm::vec2* uvs)
{
    if (data->QuadIndexCount + data->NumIndices >= data->MaxIndices)
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
        data->QuadVertexPtr->pos = transform * data->QuadVertexPositions[i];
        data->QuadVertexPtr->color = color;
        data->QuadVertexPtr->texture_coords = uvs[i];
        data->QuadVertexPtr->texture_index = texture_index;
        data->QuadVertexPtr++;
    }

    data->QuadIndexCount += 6;
}

internal void renderer2D_draw_rect(Renderer2D_Data* data, const glm::mat4& transform, const glm::vec4& color)
{
    if (data->LineIndexCount + 8 >= data->MaxVertices)
    {
        renderer2D_flush(data);
    }

    glm::vec3 lines[4];
    for (uint8_t i = 0; i < 4; i++)
    {
        lines[i] = transform * data->QuadVertexPositions[i];
    }

    renderer2D_draw_line(data, lines[0], lines[1], color);
    renderer2D_draw_line(data, lines[1], lines[2], color);
    renderer2D_draw_line(data, lines[2], lines[3], color);
    renderer2D_draw_line(data, lines[3], lines[0], color);
}

internal void renderer2D_draw_string(Renderer2D_Data* data, const Font& font, const char* string, const glm::mat4& transform, const glm::vec4& color)
{
    renderer2D_draw_string_sized(data, font, string, strlen(string), transform, color);
}

internal void renderer2D_draw_string_sized(Renderer2D_Data* data, const Font& font, const char* string, size_t string_size, const glm::mat4& transform, const glm::vec4& color)
{
    if (data->TextIndexCount + data->NumIndices * string_size >= data->MaxIndices)
    {
        renderer2D_flush(data);
    }

    const auto& fontGeometry = font.font_data->FontGeometry;
    const auto& metrics = fontGeometry.getMetrics();

    float texture_index = -1.0f;
    for (uint32_t i = 0; i < data->TextureSlotIndex; i++)
    {
        if (data->TextureSlots[i].renderer_id == font.font_atlas.renderer_id)
        {
            texture_index = (float)i;
            break;
        }
    }

    if (texture_index == -1.0f && data->TextureSlotIndex < data->MaxTextureSlots)
    {
        texture_index = (float)data->TextureSlotIndex;
        data->TextureSlots[data->TextureSlotIndex++] = font.font_atlas;
    }

    double x = 0.0;
    double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
    double y = 0.0;

    const float spaceGlyphAdvance = (float)fontGeometry.getGlyph(' ')->getAdvance();

    for (size_t i = 0; i < string_size; i++)
    {
        char character = string[i];

        if (character == '\r')
            continue;

        if (character == '\n')
        {
            x = 0;
            y -= fsScale * metrics.lineHeight + font.line_spacing;
            continue;
        }

        if (character == ' ')
        {
            float advance = spaceGlyphAdvance;
            if (i < string_size - 1)
            {
                char nextCharacter = string[i + 1];
                double dAdvance;
                fontGeometry.getAdvance(dAdvance, character, nextCharacter);
                advance = (float)dAdvance;
            }

            x += fsScale * advance + font.kerning;
            continue;
        }

        if (character == '\t')
        {
            x += 4.0f * (fsScale * spaceGlyphAdvance + font.kerning);
            continue;
        }

        auto glyph = fontGeometry.getGlyph(character);
        if (!glyph)
            glyph = fontGeometry.getGlyph('?');
        if (!glyph)
            return;

        double al, ab, ar, at;
        glyph->getQuadAtlasBounds(al, ab, ar, at);
        glm::vec2 texCoordMin((float)al, (float)ab);
        glm::vec2 texCoordMax((float)ar, (float)at);

        double pl, pb, pr, pt;
        glyph->getQuadPlaneBounds(pl, pb, pr, pt);
        glm::vec2 quadMin((float)pl, (float)pb);
        glm::vec2 quadMax((float)pr, (float)pt);

        quadMin *= fsScale, quadMax *= fsScale;
        quadMin += glm::vec2(x, y);
        quadMax += glm::vec2(x, y);

        float texelWidth = 1.0f / font.font_atlas.width;
        float texelHeight = 1.0f / font.font_atlas.height;
        texCoordMin *= glm::vec2(texelWidth, texelHeight);
        texCoordMax *= glm::vec2(texelWidth, texelHeight);

        data->TextVertexPtr->pos = transform * glm::vec4(quadMin, 0.0f, 1.0f);
        data->TextVertexPtr->color = color;
        data->TextVertexPtr->texture_index = texture_index;
        data->TextVertexPtr->texture_coords = texCoordMin;
        data->TextVertexPtr++;

        data->TextVertexPtr->pos = transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f);
        data->TextVertexPtr->color = color;
        data->TextVertexPtr->texture_index = texture_index;
        data->TextVertexPtr->texture_coords = { texCoordMin.x, texCoordMax.y };
        data->TextVertexPtr++;

        data->TextVertexPtr->pos = transform * glm::vec4(quadMax, 0.0f, 1.0f);
        data->TextVertexPtr->color = color;
        data->TextVertexPtr->texture_index = texture_index;
        data->TextVertexPtr->texture_coords = texCoordMax;
        data->TextVertexPtr++;

        data->TextVertexPtr->pos = transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f);
        data->TextVertexPtr->color = color;
        data->TextVertexPtr->texture_index = texture_index;
        data->TextVertexPtr->texture_coords = { texCoordMax.x, texCoordMin.y };
        data->TextVertexPtr++;

        data->TextIndexCount += 6;

        if (i < string_size - 1)
        {
            double advance = glyph->getAdvance();
            char nextCharacter = string[i + 1];
            fontGeometry.getAdvance(advance, character, nextCharacter);

            x += fsScale * advance + font.kerning;
        }
    }
}

internal void renderer2D_draw_line(Renderer2D_Data* data, const glm::vec2& p0, const glm::vec2& p1, const glm::vec4& color)
{
    if (data->LineIndexCount + 2 >= data->MaxVertices)
    {
        renderer2D_flush(data);
    }

    glm::vec3 p0_vec3 = { p0.x, p0.y, 0.0f };
    glm::vec3 p1_vec3 = { p1.x, p1.y, 0.0f };

    data->LineVertexPtr->pos = p0_vec3;
    data->LineVertexPtr->color = color;
    data->LineVertexPtr++;

    data->LineVertexPtr->pos = p1_vec3;
    data->LineVertexPtr->color = color;
    data->LineVertexPtr++;

    data->LineIndexCount += 2;
}

