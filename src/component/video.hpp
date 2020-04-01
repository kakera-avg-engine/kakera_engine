#ifndef KAKERA_ENGINE_COMPONENT_VIDEO
#define KAKERA_ENGINE_COMPONENT_VIDEO

#include "../graphic/shader_normal.hpp"
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}

#include "component_base.hpp"

// Video type id = 4

class Video : public Component
{
private:
    std::unique_ptr<RenderObject> render_obj;
    AVFormatContext* format_ctx = nullptr;
public:
    Video()
    {
        render_obj = std::make_unique<RenderObject>();
        render_obj->set_shader(&KAKERA_SHADER_NORMAL);
    }

    ~Video()
    {

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

    void set_src(std::string src)
    {
        avformat_open_input(&format_ctx, src.c_str(), nullptr, nullptr);

    }

    void render() override
    {

    }
};

#endif // !KAKERA_ENGINE_COMPONENT_VIDEO
