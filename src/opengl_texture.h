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

Texture2D texture_create(uint32_t width, uint32_t height, GLenum internalFormat, GLenum dataFormat, void* data);
Texture2D texture_create_from_file(const char* filepath);
void texture_bind(Texture2D texture, uint32_t slot);

Texture2D texture_create(uint32_t width, uint32_t height, GLenum internalFormat, GLenum dataFormat, void* data)
{
    Texture2D texture = {
        .width = width,
        .height = height
    };

    glCheckError(glCreateTextures(GL_TEXTURE_2D, 1, &texture.renderer_id));
    glCheckError(glTextureStorage2D(texture.renderer_id, 1, internalFormat, texture.width, texture.height));

    glCheckError(glTextureParameteri(texture.renderer_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    glCheckError(glTextureParameteri(texture.renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    glCheckError(glTextureParameteri(texture.renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glCheckError(glTextureParameteri(texture.renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT));

    glCheckError(glTextureSubImage2D(texture.renderer_id, 0, 0, 0, texture.width, texture.height, dataFormat, GL_UNSIGNED_BYTE, data));

    return texture;
}

// TODO: allow for choosing filtering type
Texture2D texture_create_from_file(const char* filepath)
{

    // NOTE: load data for texture
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned char* textureData = stbi_load(filepath, &width, &height, &channels, 0);

    GLenum internalFormat = 0;
    GLenum dataFormat = 0;
    if (channels == 4)
    {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }
    else if (channels == 3)
    {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }

    Texture2D texture = texture_create(width, height, internalFormat, dataFormat, textureData);

    stbi_image_free(textureData);

    return texture;
}

void texture_bind(Texture2D texture, uint32_t slot)
{
    glCheckError(glBindTextureUnit(slot, texture.renderer_id));
}

#endif // OPENGL_TEXTURE_H
