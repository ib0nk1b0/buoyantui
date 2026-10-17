#pragma once

struct MSDFData
{
    std::vector<msdf_atlas::GlyphGeometry> Glyphs;
    msdf_atlas::FontGeometry FontGeometry;
};

struct Font
{
    MSDFData* font_data;
    Texture2D font_atlas;
    float     line_spacing = 0.0f;
    float     kerning = 0.0f;
};

internal Font font_load_from_file(Arena* arena, const char* path);
internal float font_calculate_string_length(const Font& font, const char* string);
internal float font_calculate_string_length_sized(const Font& font, const char* string, uint32_t size);
