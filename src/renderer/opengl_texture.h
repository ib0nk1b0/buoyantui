#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#define FONT_WIDTH 128
#define FONT_HEIGHT 64
#define FONT_COLS 18
#define FONT_ROWS 7
#define FONT_CHAR_WIDTH (FONT_WIDTH / FONT_COLS)
#define FONT_CHAR_HEIGHT (FONT_HEIGHT / FONT_ROWS)

typedef struct
{
    uint32_t renderer_id;
    uint32_t width;
    uint32_t height;
    uint32_t channels;
} Texture2D;

static Texture2D texture_create(uint32_t width, uint32_t height, GLenum internalFormat, GLenum dataFormat, void* data);
static Texture2D texture_create_from_file(const char* filepath);
static void texture_bind(Texture2D texture, uint32_t slot);

#endif // OPENGL_TEXTURE_H
