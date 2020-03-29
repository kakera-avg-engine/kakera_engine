#ifndef KAKERA_ENGINE_COMPONENT_IMAGE
#define KAKERA_ENGINE_COMPONENT_IMAGE

#include "component_base.hpp"
#include "../graphic/texture_manager.h"
#include "../file_package/package_manager.h"
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
        render_obj = std::make_unique<RenderObject>();
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
        if (auto image_file = KAKERA_PACKAGE_MANAGER.get_file(src); image_file.has_value()) {

            Texture* texture = KAKERA_TEXTURE_MANAGER[src];

            if (!texture) {
                File file = std::move(image_file.value());
                SDL_RWops* sdl_file = SDL_RWFromMem(*file, file.size());
                SDL_Surface* image_surface = IMG_Load_RW(sdl_file, 1);
                if (image_surface) {
                    TextureFormat format = TextureFormat::rgb;
                    if (image_surface->format->BytesPerPixel == 4)
                        format = TextureFormat::rgba;
                    KAKERA_TEXTURE_MANAGER.set_texture(src, Texture(image_surface->w, image_surface->h, format));
                    texture = KAKERA_TEXTURE_MANAGER[src];
                    texture->update_pixels((unsigned char*)image_surface->pixels);
                    SDL_FreeSurface(image_surface);
                }
            }

            render_obj->set_texture(texture);
        }
    }

    void render() override
    {
        render_obj->render();
    }
};

#endif // !KAKERA_ENGINE_COMPONENT_IMAGE
