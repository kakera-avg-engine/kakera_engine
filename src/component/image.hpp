#ifndef KAKERA_ENGINE_COMPONENT_IMAGE
#define KAKERA_ENGINE_COMPONENT_IMAGE

#include "component_base.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "../graphic/shader_normal.hpp"

// Image type id = 1

class Image : public Component
{
private:
    std::unique_ptr<RenderObject> render_obj;
public:
    KAKERA_DISABLE_COPY(Image);

    Image()
    {
        render_obj = std::make_unique<RenderObject>(false);
        render_obj->set_shader(&KAKERA_SHADER_NORMAL);
    }

    Image(Image&& other)
    {
        width = other.width;
        height = other.height;
        x = other.x;
        y = other.y;
        uuid = other.uuid;

        other.width = 0;
        other.height = 0;
        other.x = 0;
        other.y = 0;
        other.uuid = "";

        render_obj = std::move(other.render_obj);
    }

    ~Image() = default;

    Image& operator=(Image&& other)
    {
        if (&other != this) {
            width = other.width;
            height = other.height;
            x = other.x;
            y = other.y;
            uuid = other.uuid;

            other.width = 0;
            other.height = 0;
            other.x = 0;
            other.y = 0;
            other.uuid = "";

            render_obj = std::move(other.render_obj);
        }

        return *this;
    }

    void set_size(int width, int height) override
    {
        Component::set_size(width, height);
        render_obj->set_size(width, height);
    }

    void set_position(int x, int y) override
    {
        Component::set_position(x, y);
        render_obj->set_position(x, y);
    }

    void set_src(const char* src)
    {
        SDL_Surface* image_surface = IMG_Load(src);
        TextureFormat format = TextureFormat::rgb;
        if (image_surface->format->BytesPerPixel == 4) format == TextureFormat::rgba;
        Texture texture(image_surface->w, image_surface->h, format);
        texture.update_pixels((unsigned char*)image_surface->pixels);
        SDL_FreeSurface(image_surface);
        render_obj->set_texture(std::move(texture));
    }

    void render() override
    {
        render_obj->render();
    }
};

#endif // !KAKERA_ENGINE_COMPONENT_IMAGE
