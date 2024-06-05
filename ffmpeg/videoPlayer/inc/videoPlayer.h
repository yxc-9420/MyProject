#ifndef BXC_VIDEOPLAYER_H
#define BXC_VIDEOPLAYER_H

#include <mutex>
#include <list>
#include <thread>
#include <vector>

#define  SDL_MAIN_HANDLED
#include <SDL.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
}

#define MAX_AUDIO_SIZE (50 * 20)
#define MAX_VIDEO_SIZE (25 * 20)

class VideoPlayer {
public:
    explicit VideoPlayer();
    ~VideoPlayer();
public:
    int start(const char* input);
private:

    AVFormatContext* mFmtCtx;
    static void decodeVideoThread(void* arg);
    void handleVideo();
    static void decodeAudioThread(void* arg);
    void handleAudio();
    std::vector<std::thread*> mThreads;
    // 视频
    AVCodecContext * mVideoCodecCtx;
    std::list <AVPacket> mVideoPktList;
    std::mutex           mVideoPktList_mtx;
    bool pushVideoPkt(const AVPacket &pkt);
    void clearVideoPkt();
    // 音频
    AVCodecContext * mAudioCodecCtx;
    std::list <AVPacket> mAudioPktList;
    std::mutex           mAudioPktList_mtx;
    AVStream* mAudioStream;
    bool pushAudioPkt(const AVPacket &pkt);
    void clearAudioPkt();


private:
    int initSDL();
    SDL_Window   *sdl_window; // 窗口
    SDL_Renderer * sdl_renderer;// 渲染器
    SDL_Texture  *sdl_texture; // 纹理
    int sdl_window_width = 720; // 窗口宽度
    int sdl_window_height = 480; // 窗口高度
    bool bDone = false;
    static void audioCallBack(void* userdata, uint8_t *stream, int len);
};

#endif //BXC_VIDEOPLAYER_H