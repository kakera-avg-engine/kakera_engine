#ifndef KAKERA_ENGINE_COMPONENT_VIDEO
#define KAKERA_ENGINE_COMPONENT_VIDEO

#include "../graphic/shader_normal.hpp"
#include "../file_package/package_manager.h"
#include "../graphic/texture_manager.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <chrono>
#include <cmath>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}
#include "SDL.h"

#include "component_base.hpp"

#ifdef _WIN32
#   undef min
#endif

#include <iostream>

// Video type id = 4

class Video : public Component
{
private:
    std::unique_ptr<RenderObject> render_obj;
    Uint32 interval = 0;
    
    AVFormatContext* format_ctx = nullptr;
    AVCodecContext* codec_ctx = nullptr;
    AVPacket packet;
    AVFrame* raw_frame = nullptr;
    AVFrame* rgb_frame = nullptr;
    SwsContext* sws_ctx = nullptr;

    int video_stream_index = -1;
    int audio_stream_index = -1;

    int queue_cache_size = 0;
    unsigned char* video_buffer = nullptr;
    int buffer_size = 0;
    std::queue<std::unique_ptr<unsigned char>> video_frame_cache;

    void video_decode()
    {
        while (true) {
            if (video_frame_cache.size() >= queue_cache_size)
                std::this_thread::sleep_for(std::chrono::milliseconds(16));

            //printf("%x\n", this);
            if (this) {
                if (av_read_frame(format_ctx, &packet) >= 0) {
                    if (packet.stream_index == video_stream_index) {
                        avcodec_send_packet(codec_ctx, &packet);
                        if (avcodec_receive_frame(codec_ctx, raw_frame) != 0)
                            continue;
                        sws_scale(sws_ctx, raw_frame->data, raw_frame->linesize, 0, codec_ctx->height, rgb_frame->data, rgb_frame->linesize);

                        auto video_frame_data = std::unique_ptr<unsigned char>(new unsigned char[buffer_size]);
                        memcpy(video_frame_data.get(), video_buffer, buffer_size);

                        video_frame_cache.push(std::move(video_frame_data));

                    }
                    av_packet_unref(&packet);
                }
                else {
                    packet.data = nullptr;
                    packet.size = 0;
                    av_read_frame(format_ctx, &packet);
                    if (packet.stream_index == video_stream_index) {
                        avcodec_send_packet(codec_ctx, &packet);
                        avcodec_receive_frame(codec_ctx, raw_frame);

                        sws_scale(sws_ctx, raw_frame->data, raw_frame->linesize, 0, codec_ctx->height, rgb_frame->data, rgb_frame->linesize);

                        auto video_frame_data = std::unique_ptr<unsigned char>(new unsigned char[buffer_size]);
                        memcpy(video_frame_data.get(), video_buffer, buffer_size);

                        video_frame_cache.push(std::move(video_frame_data));
                    }
                    av_packet_unref(&packet);
                    break;
                }
            }
            else
                break;
        }
    }
public:
    Video()
    {
        render_obj = std::make_unique<RenderObject>();
        render_obj->set_shader(&KAKERA_SHADER_NORMAL);
        raw_frame = av_frame_alloc();
        rgb_frame = av_frame_alloc();
    }

    ~Video()
    {
        av_free(video_buffer);
        sws_freeContext(sws_ctx);
        av_frame_free(&rgb_frame);
        av_frame_free(&raw_frame);
        avcodec_close(codec_ctx);
        avformat_close_input(&format_ctx);
        KAKERA_TEXTURE_MANAGER.destroy_texture(uuid);
    }

    void set_size(int width, int height) override
    {
        Component::set_size(width, height);
        render_obj->set_size(width, height);
        render_obj->set_texture(KAKERA_TEXTURE_MANAGER.set_texture(uuid, Texture(width, height)));
    }

    void set_position(int x, int y) override
    {
        Component::set_position(x, y);
        render_obj->set_position(x, y);
    }

    void set_src(std::string src)
    {
        avformat_open_input(&format_ctx, src.c_str(), nullptr, nullptr);
        avformat_find_stream_info(format_ctx, nullptr);
        for (int i = 0; i < format_ctx->nb_streams; i++) {
            if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                video_stream_index = i;
            }
            else if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                audio_stream_index = i;
            }
        }
        queue_cache_size = (int)av_q2d(format_ctx->streams[video_stream_index]->avg_frame_rate);
        AVCodec* codec = avcodec_find_decoder(format_ctx->streams[video_stream_index]->codecpar->codec_id);
        codec_ctx = avcodec_alloc_context3(codec);
        avcodec_parameters_to_context(codec_ctx, format_ctx->streams[video_stream_index]->codecpar);
        avcodec_open2(codec_ctx, codec, nullptr);
        buffer_size = av_image_get_buffer_size(AV_PIX_FMT_RGB24, width, height, 1);
        video_buffer = (unsigned char*)av_malloc(buffer_size);
        av_image_fill_arrays(rgb_frame->data, rgb_frame->linesize, video_buffer, AV_PIX_FMT_RGB24, width, height, 1);
        sws_ctx = sws_getContext(codec_ctx->width, codec_ctx->height, codec_ctx->pix_fmt, width, height, AV_PIX_FMT_RGB24, SWS_BILINEAR, nullptr, nullptr, nullptr);
        std::thread video_decode_thread(&Video::video_decode, this);
        video_decode_thread.detach();
    }

    void render() override
    {
        if (!video_frame_cache.empty()) {
            auto video_frame_data = std::move(video_frame_cache.front());
            video_frame_cache.pop();
            Texture* texture = render_obj->get_texture();
            texture->update_pixels(video_frame_data.get());
        }

        render_obj->render();
    }
};

#endif // !KAKERA_ENGINE_COMPONENT_VIDEO
