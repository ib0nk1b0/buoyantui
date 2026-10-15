#include "opengl_font.h"

#define DEFAULT_ANGLE_THRESHOLD 3.0
#define LCG_MULTIPLIER 6364136223846793005ull
#define LCG_INCREMENT 1442695040888963407ull
#define THREAD_COUNT 8
// NOTE:This is bad ^

internal Font font_load_from_file(Arena* arena, const char* path)
{
    MSDFData* m_Data = ArenaPushStruct(arena, MSDFData);
    m_Data = new (m_Data) MSDFData{};
    msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
    msdfgen::FontHandle* font = msdfgen::loadFont(ft, path);

    if (!font)
    {
        printf("Failed to load font");
    }

    struct CharsetRange
    {
        uint32_t Begin, End;
    };

    // From imgui_draw.cpp
    static const CharsetRange charsetRanges[] =
    {
        { 0x0020, 0x00FF }
    };

    msdf_atlas::Charset charset;
    for (CharsetRange range : charsetRanges)
    {
        for (uint32_t c = range.Begin; c <= range.End; c++)
            charset.add(c);
    }

    double fontScale = 1.0;
    m_Data->FontGeometry = msdf_atlas::FontGeometry(&m_Data->Glyphs);
    int glyphsLoaded = m_Data->FontGeometry.loadCharset(font, fontScale, charset);
    printf("Loaded %d glyphs from font (out of %zu)", glyphsLoaded, charset.size());

    double emSize = 40.0;

    msdf_atlas::TightAtlasPacker atlasPacker;
    atlasPacker.setPixelRange(2.0);
    atlasPacker.setMiterLimit(1.0);
    atlasPacker.setSpacing(0); // might not be right
    atlasPacker.setScale(emSize);
    int remaining = atlasPacker.pack(m_Data->Glyphs.data(), (int)m_Data->Glyphs.size());
    assert(remaining == 0);

    int atlas_width, atlas_height;
    atlasPacker.getDimensions(atlas_width, atlas_height);
    emSize = atlasPacker.getScale();


    uint64_t coloringSeed = 0;
    bool expensiveColoring = false;
    if (expensiveColoring)
    {
        // msdf_atlas::Workload([&glyphs = m_Data->Glyphs, &coloringSeed](int i, int threadNo) -> bool {
        //         unsigned long long glyphSeed = (LCG_MULTIPLIER * (coloringSeed ^ i) + LCG_INCREMENT) * !!coloringSeed;
        //         glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
        //         return true;
        //         }, m_Data->Glyphs.size()).finish(THREAD_COUNT);
    }
    else {
        unsigned long long glyphSeed = coloringSeed;
        for (msdf_atlas::GlyphGeometry& glyph : m_Data->Glyphs)
        {
            glyphSeed *= LCG_MULTIPLIER;
            glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
        }
    }

    msdf_atlas::GeneratorAttributes attributes;
    attributes.config.overlapSupport = true;
    attributes.scanlinePass = true;

    msdf_atlas::ImmediateAtlasGenerator<float, 3, msdf_atlas::msdfGenerator, msdf_atlas::BitmapAtlasStorage<uint8_t, 3>> generator(atlas_width, atlas_height);
    generator.setAttributes(attributes);
    generator.setThreadCount(THREAD_COUNT);
    generator.generate(m_Data->Glyphs.data(), (int)m_Data->Glyphs.size());

    msdfgen::BitmapConstRef<uint8_t, 3> bitmap = (msdfgen::BitmapConstRef<uint8_t, 3>)generator.atlasStorage();

    Texture2D atlas = texture_create(atlas_width, atlas_height, GL_RGB8, GL_RGB, (void*)bitmap.pixels, false);

    msdfgen::destroyFont(font);
    msdfgen::deinitializeFreetype(ft);

    Font result = {};
    result.font_data = m_Data;
    result.font_atlas = atlas;
    return result;
}
