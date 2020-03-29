#include "texture.h"

Texture::Texture(int width, int height, TextureFormat format)
{
    w = width;
    h = height;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (format != TextureFormat::rgb) {
        if (format == TextureFormat::red) 
            gl_format = GL_RED;
        else if (format == TextureFormat::rgba) 
            gl_format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, gl_format, GL_UNSIGNED_BYTE, nullptr);
}

Texture::Texture(Texture&& other)
{
    w = other.w;
    h = other.h;
    tex_id = other.tex_id;
    gl_format = other.gl_format;

    other.w = 0;
    other.h = 0;
    other.tex_id = UINT_MAX;
    other.gl_format = GL_RGB;
}

Texture::~Texture()
{
    if (glIsTexture(tex_id)) {
        glDeleteTextures(1, &tex_id);
    }
}

Texture& Texture::operator=(Texture&& other)
{
    if (&other != this) {
        w = other.w;
        h = other.h;
        tex_id = other.tex_id;
        gl_format = other.gl_format;

        other.w = 0;
        other.h = 0;
        other.tex_id = UINT_MAX;
        other.gl_format = GL_RGB;
    }

    return *this;
}

void Texture::update_pixels(unsigned char* pixels)
{
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, gl_format, GL_UNSIGNED_BYTE, pixels);
}

void Texture::update_pixels(int x, int y, int width, int height, unsigned char* pixels)
{
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, gl_format, GL_UNSIGNED_BYTE, pixels);
}

const GLuint Texture::get_id()
{
    return tex_id;
}

std::array<GLfloat, 12> Texture::get_area_coord(SDL_Rect rect)
{
    if (rect.x + rect.w > w || rect.y + rect.h > h) {
        KAKERA_LOG(LogLevel::Error, "Select area is larger than texture size.");
        return std::array<GLfloat, 12>();
    }

    GLfloat left_x, right_x, top_y, bottom_y;

    left_x = (GLfloat)rect.x / (GLfloat)w;
    right_x = (GLfloat)(rect.x + rect.w) / (GLfloat)w;
    top_y = (GLfloat)(h - rect.y) / (GLfloat)h;
    bottom_y = (GLfloat)(h - rect.y - rect.h) / (GLfloat)h;

    std::array<GLfloat, 12> result = {
        left_x, top_y, right_x, bottom_y,
        left_x, bottom_y, left_x, top_y,
        right_x, top_y, right_x, bottom_y
    };

    return result;
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, tex_id);
}
