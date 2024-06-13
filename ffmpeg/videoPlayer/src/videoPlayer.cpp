#include "../inc/videoPlayer.h"
#include "../inc/Log.h"
#pragma warning(disable : 4996)

// Buffer:
static int audio_len = 0;
static Uint8 *audio_pos;

void VideoPlayer::audioCallBack(void *userdata, uint8_t *stream, int len)
{

    SDL_memset(stream, 0, len);
    if (audio_len <= 0)
        return;

    len = (len > audio_len ? audio_len : len); /*  Mix  as  much  data  as  possible  */

    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
    audio_pos += len;
    audio_len -= len;
}

VideoPlayer::VideoPlayer()
{
    LOGI("VideoPlayer::VideoPlayer()");
    initSDL();
}
VideoPlayer::~VideoPlayer()
{
    LOGI("VideoPlayer::~VideoPlayer() start");

    for (auto th : mThreads)
    {
        th->join();
    }
    for (auto th : mThreads)
    {
        delete th;
        th = nullptr;
    }
    mThreads.clear();

    clearVideoPkt();
    clearAudioPkt();

    if (mVideoCodecCtx)
    {

        avcodec_close(mVideoCodecCtx);
        avcodec_free_context(&mVideoCodecCtx);
        mVideoCodecCtx = NULL;
    }

    if (mAudioCodecCtx)
    {
        avcodec_close(mAudioCodecCtx);
        avcodec_free_context(&mAudioCodecCtx);
        mAudioCodecCtx = NULL;
    }

    if (mFmtCtx)
    {
        // 拉流不需要释放start
        // if (mFmtCtx && !(mFmtCtx->oformat->flags & AVFMT_NOFILE)) {
        //    avio_close(mFmtCtx->pb);
        //}
        // 拉流不需要释放end
        avformat_close_input(&mFmtCtx);
        avformat_free_context(mFmtCtx);
        mFmtCtx = NULL;
    }

    if (sdl_window)
    {
        SDL_DestroyWindow(sdl_window);
    }

    if (sdl_renderer)
    {
        SDL_DestroyRenderer(sdl_renderer);
    }

    if (sdl_texture)
    {
        SDL_DestroyTexture(sdl_texture);
    }

    SDL_Quit();
    LOGI("VideoPlayer::~VideoPlayer() end");
}
int VideoPlayer::start(const char *input)
{

    mFmtCtx = avformat_alloc_context();

    if (avformat_open_input(&mFmtCtx, input, nullptr, nullptr) != 0)
    {
        printf("无法打开视频文件 \n");
        return -1;
    }
    if (avformat_find_stream_info(mFmtCtx, nullptr) < 0)
    {
        printf("无法获取视频流信息 \n");
        return -1;
    }

    int videoIndex = -1;
    int audioIndex = -1;

    videoIndex = av_find_best_stream(mFmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (videoIndex >= 0)
    {

        AVCodecParameters *videoPar = mFmtCtx->streams[videoIndex]->codecpar;
        const AVCodec *videoCodec = avcodec_find_decoder(videoPar->codec_id);
        // AVCodec* videoCodec = avcodec_find_decoder_by_name("h264_cuvid");

        if (!videoCodec)
        {
            printf("视频解码器不存在 \n");
            return -1;
        }
        mVideoCodecCtx = avcodec_alloc_context3(videoCodec);
        if (avcodec_parameters_to_context(mVideoCodecCtx, videoPar) != 0)
        {
            printf("视频解码器无法copy上下文 \n");
            return -1;
        }
        if (avcodec_open2(mVideoCodecCtx, videoCodec, nullptr) < 0)
        {
            printf("视频解码器无法打开 \n");
            return -1;
        }
        std::thread *th = new std::thread(VideoPlayer::decodeVideoThread, this);
        mThreads.push_back(th);
    }
    else
    {
        printf("未找到视频流 \n");

        return -1;
    }

    audioIndex = av_find_best_stream(mFmtCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (audioIndex >= 0)
    {
        AVCodecParameters *audioPar = mFmtCtx->streams[audioIndex]->codecpar;
        const AVCodec *audioCodec = avcodec_find_decoder(audioPar->codec_id);

        if (!audioCodec)
        {
            printf("音频解码器不存在 \n");
        }

        mAudioCodecCtx = avcodec_alloc_context3(audioCodec);

        if (avcodec_parameters_to_context(mAudioCodecCtx, audioPar) != 0)
        {
            printf("音频解码器无法copy上下文 \n");
        }

        if (avcodec_open2(mAudioCodecCtx, audioCodec, nullptr) < 0)
        {
            printf("音频解码器无法打开 \n");
        }

        mAudioStream = mFmtCtx->streams[audioIndex];

        std::thread *th = new std::thread(VideoPlayer::decodeAudioThread, this);
        mThreads.push_back(th);
    }
    else
    {
        printf("未找到音频流 \n");
    }

    AVStream *videoStream = mFmtCtx->streams[videoIndex];
    int videoFps = videoStream->avg_frame_rate.num / videoStream->avg_frame_rate.den;
    LOGI("videoFps=%d", videoFps);

    // for (auto th : mThreads) {
    //     th->native_handle();
    // }
    AVPacket pkt;
    int64_t t1, t2;

    SDL_Event event;
    while (true)
    {

        // SDL_WaitEvent(&event);
        if (true)
        {
            if (av_read_frame(mFmtCtx, &pkt) >= 0)
            {
                t1 = getCurTimestamp();
                if (videoIndex == pkt.stream_index)
                {
                    pushVideoPkt(pkt);
                }
                else if (audioIndex == pkt.stream_index)
                {
                    pushAudioPkt(pkt);
                }
                else
                {
                    av_packet_unref(&pkt);
                }
                t2 = getCurTimestamp();
            }
            else
            {
                bDone = true;
            }
            // }
            // else if (event.type == SDL_USEREVENT) {
            // }
            // else if (event.type == SDL_QUIT) {
            //  break;
            // }
            // else {
        }
        // 这里做了个限制  当队列里面的数据超过某个大小的时候 就暂停读取  防止一下子就把视频读完了，导致的空间分配不足
        // 这个值可以稍微写大一些
        if (mVideoPktList.size() > MAX_VIDEO_SIZE || mAudioPktList.size() > MAX_AUDIO_SIZE)
        {
            // Sleep(10);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            // std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
    }

    return 0;
}
void VideoPlayer::handleVideo()
{
    AVFrame *frame = av_frame_alloc();
    AVFrame *sw_frame = av_frame_alloc();
    sw_frame->format = AV_PIX_FMT_YUV420P;

    sw_frame->width = sdl_window_width;
    sw_frame->height = sdl_window_height;
    int sw_frame_buff_size = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, sw_frame->width, sw_frame->height, 1);
    uint8_t *sw_frame_buff = (uint8_t *)av_malloc(sw_frame_buff_size);
    av_image_fill_arrays(sw_frame->data, sw_frame->linesize, sw_frame_buff, AV_PIX_FMT_YUV420P, sw_frame->width, sw_frame->height, 1);

    int ret;
    while (true)
    {

        mVideoPktList_mtx.lock();
        if (mVideoPktList.empty())
        {
            mVideoPktList_mtx.unlock();
            if (bDone)
            {
                break;
            }
        }
        else
        {
            AVPacket pkt = mVideoPktList.front();

            mVideoPktList.pop_front();
            mVideoPktList_mtx.unlock();

            avcodec_send_packet(mVideoCodecCtx, &pkt);

            // int virtualpts = pkt.pts;
            // int realpts = av_rescale_q(virtualpts,mFmtCtx->streams[pkt.stream_index]->time_base,{1, AV_TIME_BASE});

            while (true)
            {
                ret = avcodec_receive_frame(mVideoCodecCtx, frame);
                if (ret >= 0)
                {
                    auto img_convert_ctx = sws_getContext(frame->width, frame->height, (enum AVPixelFormat)frame->format,
                                                          sw_frame->width, sw_frame->height, (enum AVPixelFormat)sw_frame->format, SWS_BICUBIC, NULL, NULL, NULL);

                    sws_scale(img_convert_ctx, (const unsigned char *const *)frame->data,
                              frame->linesize, 0, frame->height,
                              sw_frame->data, sw_frame->linesize);
                    sws_freeContext(img_convert_ctx);

                    SDL_UpdateYUVTexture(sdl_texture, nullptr,
                                         sw_frame->data[0], sw_frame->linesize[0],
                                         sw_frame->data[1], sw_frame->linesize[1],
                                         sw_frame->data[2], sw_frame->linesize[2]);

                    SDL_Rect srcrect;
                    srcrect.x = 0;
                    srcrect.y = 0;
                    srcrect.w = sw_frame->width;
                    srcrect.h = sw_frame->height;

                    SDL_Rect dstrect;
                    dstrect.x = 0;
                    dstrect.y = 0;
                    dstrect.w = sdl_window_width;
                    dstrect.h = sdl_window_height;

                    SDL_RenderClear(sdl_renderer);
                    SDL_RenderCopy(sdl_renderer, sdl_texture, &srcrect, &dstrect);
                    SDL_RenderPresent(sdl_renderer);

                    av_packet_unref(&pkt);

                    std::this_thread::sleep_for(std::chrono::milliseconds(40));
                }
                else
                {
                    break;
                }
            }
        }
    }

    av_frame_free(&frame);
    frame = nullptr;
    free(sw_frame_buff);
    sw_frame_buff = nullptr;
    av_frame_free(&sw_frame);
    sw_frame = nullptr;
}
void VideoPlayer::decodeVideoThread(void *arg)
{
    VideoPlayer *player = (VideoPlayer *)arg;
    player->handleVideo();
}

void VideoPlayer::handleAudio()
{
    // 重采样
    // AVChannelLayout in_channel_layout = AV_CHANNEL_LAYOUT_STEREO;
    uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO; // 输出声道
    // out_channel_layout = AV_CH_SIDE_LEFT;
    int out_channels = 2; // av_get_channel_layout_nb_channels(out_channel_layout);
    int out_nb_samples = 1024;
    AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16; // 输出格式S16
    int out_sample_rate = 44100;

    int out_buffer_size = av_samples_get_buffer_size(nullptr,
                                                     out_channels,
                                                     out_nb_samples,
                                                     out_sample_fmt,
                                                     1);
    uint8_t *out_buffer = (uint8_t *)av_malloc(out_buffer_size);

    SDL_AudioSpec spec;
    spec.freq = out_sample_rate;
    spec.format = AUDIO_S16SYS;
    spec.channels = out_channels;
    spec.silence = 0;
    spec.samples = out_nb_samples;
    spec.callback = VideoPlayer::audioCallBack;
    spec.userdata = this;

    if (SDL_OpenAudio(&spec, nullptr) < 0)
    {
        LOGE("SDL_OpenAudio error");
        return;
    }
    SDL_PauseAudio(0); // 当pause_on设置为0的时候即可开始播放音频数据。设置为1的时候，将会播放静音的值

    // uint64_t in_channel_layout = av_get_default_channel_layout(mAudioCodecCtx->channels);
    AVFrame *in_frame = av_frame_alloc();
    AVSampleFormat in_sample_fmt = mAudioCodecCtx->sample_fmt;
    int in_sample_rate = mAudioCodecCtx->sample_rate;

    struct SwrContext *audioSwrCtx = swr_alloc();
    // 输出
    AVChannelLayout outChannelLayout = AV_CHANNEL_LAYOUT_STEREO;
    // 输入
    AVChannelLayout inChannelLayout;
    inChannelLayout = mAudioCodecCtx->ch_layout;
    int ret = swr_alloc_set_opts2(&audioSwrCtx,
                                  &outChannelLayout,
                                  out_sample_fmt,
                                  out_sample_rate,
                                  &inChannelLayout,
                                  in_sample_fmt,
                                  in_sample_rate,
                                  0, nullptr);

    swr_init(audioSwrCtx);

    // int ret;

    while (true)
    {
        mAudioPktList_mtx.lock();
        if (mAudioPktList.empty())
        {
            mAudioPktList_mtx.unlock();
            if (bDone)
            {
                break;
            }
        }
        else
        {
            AVPacket pkt = mAudioPktList.front();
            mAudioPktList.pop_front();
            mAudioPktList_mtx.unlock();

            avcodec_send_packet(mAudioCodecCtx, &pkt);
            while (true)
            {
                ret = avcodec_receive_frame(mAudioCodecCtx, in_frame);
                if (ret >= 0)
                {
                    //重采样
                    swr_convert(audioSwrCtx, &out_buffer, out_buffer_size,
                                (const uint8_t **)in_frame->data, in_frame->nb_samples);

                    while (audio_len > 0)
                    { // Wait until finish
                        SDL_Delay(1);
                    }

                    // Set audio buffer (PCM data)
                    // Audio buffer length
                    audio_len = out_buffer_size;
                    audio_pos = (Uint8 *)out_buffer;

                    // std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
                else
                {
                    break;
                }
            }

            av_packet_unref(&pkt);
        }
    }
    swr_free(&audioSwrCtx);
    av_frame_free(&in_frame);
}
void VideoPlayer::decodeAudioThread(void *arg)
{
    VideoPlayer *player = (VideoPlayer *)arg;
    player->handleAudio();
}
int VideoPlayer::initSDL()
{
    // sdl_window_width = m_videoCodecCtx->width;
    // sdl_window_height = m_videoCodecCtx->height;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
    {
        LOGE("SDL_Init error");
        return -1;
    }

    std::string name = "BXC_VideoPlayer " + std::to_string(sdl_window_width) + "*" + std::to_string(sdl_window_height);
    // 创建SDL窗口
    if (!(sdl_window = SDL_CreateWindow(name.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sdl_window_width, sdl_window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)))
    {
        LOGE("SDL_CreateWindow error");
        return -1;
    }
    // 创建渲染器
    if (!(sdl_renderer = SDL_CreateRenderer(sdl_window, -1, 0)))
    {
        LOGE("SDL_CreateRenderer error");
        return -1;
    }
    // 创建纹理
    if (!(sdl_texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,
                                          sdl_window_width, sdl_window_height)))
    {
        LOGE("SDL_CreateTexture error");
        return -1;
    }

    // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    return 0;
}
bool VideoPlayer::pushVideoPkt(const AVPacket &pkt)
{

    if (av_packet_make_refcounted((AVPacket *)&pkt) < 0)
    {

        return false;
    }

    mVideoPktList_mtx.lock();
    mVideoPktList.push_back(pkt);
    mVideoPktList_mtx.unlock();
    return true;
}
void VideoPlayer::clearVideoPkt()
{

    mVideoPktList_mtx.lock();

    for (AVPacket pkt : mVideoPktList)
    {
        av_packet_unref(&pkt);
    }
    mVideoPktList.clear();

    mVideoPktList_mtx.unlock();
}
bool VideoPlayer::pushAudioPkt(const AVPacket &pkt)
{

    if (av_packet_make_refcounted((AVPacket *)&pkt) < 0)
    {
        return false;
    }

    mAudioPktList_mtx.lock();
    mAudioPktList.push_back(pkt);
    mAudioPktList_mtx.unlock();
    return true;
}
void VideoPlayer::clearAudioPkt()
{

    mAudioPktList_mtx.lock();

    for (AVPacket pkt : mAudioPktList)
    {
        av_packet_unref(&pkt);
    }
    mAudioPktList.clear();

    mAudioPktList_mtx.unlock();
}