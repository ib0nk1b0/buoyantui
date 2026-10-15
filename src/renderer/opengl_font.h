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
