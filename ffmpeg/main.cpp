extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavformat/avio.h>
    #include <libavutil/file.h>
    #include <libavutil/mem.h>
}
#include <string>

#include "videoPlayer/inc/videoPlayer.h"
using namespace std;

//const string g_strVideoPath = "D:/Personal/repos/MyProject/resource/粒子背景.mp4";
//const string g_strVideoPath = "D:/Personal/repos/MyProject/resource/【LIVE】花泽香菜 - 恋爱循环.mp4";
const string g_strVideoPath = "D:/Personal/repos/MyProject/resource/航拍湖北武汉鹦鹉洲长江大桥夜景_1080p.mp4";

int main()
{
    
    //播放RTSP视频流地址
    //播放本地视频文件
    VideoPlayer player;
    player.start(g_strVideoPath.c_str());
    return 0;

    //avformat_alloc_context

    // AVFormatContext* pFmtCtx = avformat_alloc_context();//分配
    // int ret = avformat_open_input(&pFmtCtx,g_strVideoPath.c_str(), NULL, NULL);
    // if(0 != ret)
    // {
    //     return;
    // }
    // ret = avformat_find_stream_info(pFmtCtx, NULL);
    // if(ret < 0)
    // {
    //     //查找输入流失败
    //     return;
    // }
    // //视频
    // int videoStream_index = -1; //视频流索引号
    // int audioStream_index = -1; //音频流索引号
    // videoStream_index = av_find_best_stream(pFmtCtx,AVMEDIA_TYPE_VIDEO, -1,-1, NULL, 0);
    // audioStream_index = av_find_best_stream(pFmtCtx,AVMEDIA_TYPE_AUDIO, -1,-1, NULL, 0);

    // if(videoStream_index < 0 || audioStream_index < 0)
    // {
    //     printf("no video or no audio");
    //     return -1;
    // }

    // //视频解码器
    // const AVCodec* pViCodec = avcodec_find_decoder(pFmtCtx->streams[videoStream_index]->codecpar->codec_id);
    // if( NULL == pViCodec)
    // {
    //     printf("视频解码器不存在");
    //     return -1;
    // }
    // AVCodecContext* pViCodecCtx = avcodec_alloc_context3(pViCodec);
    // if( NULL == pViCodecCtx)
    // {
    //     return;
    // }
    // avcodec_parameters_to_context(pViCodecCtx,pFmtCtx->streams[videoStream_index]->codecpar);
    // avcodec_open2(pViCodecCtx, pViCodec, 0);

    // //音频解码器
    // const AVCodec* pAuCodec = avcodec_find_decoder(pFmtCtx->streams[audioStream_index]->codecpar->codec_id);
    // if( NULL == pAuCodec)
    // {
    //     printf("音频解码器不存在");
    //     return -1;
    // }
    // AVCodecContext* pAuCodecCtx = avcodec_alloc_context3(pAuCodec);
    // if( NULL == pAuCodecCtx)
    // {
    //     return;
    // }
    // avcodec_parameters_to_context(pAuCodecCtx,pFmtCtx->streams[audioStream_index]->codecpar);
    // avcodec_open2(pAuCodecCtx, pAuCodec, 0);

    // av_dump_format(pFmtCtx, 0, g_strVideoPath.c_str(), 0);

    // //解码后的原始视频帧
    // AVFrame* pDeViFrm = av_frame_alloc();

    // //缩放后的是视频帧
    // AVFrame bgrFrame = {0};
    // bgrFrame.width = 960;
    // bgrFrame.height = 540;
    // bgrFrame.format = AV_PIX_FMT_BGR24;

    // int bgrFrameSize = av_image_get_buffer_size(
    //     (AVPixelFormat)bgrFrame.format,bgrFrame.width, bgrFrame.height,1);
    // uint8_t* bgrBuff = (uint8_t*)av_malloc(bgrFrameSize);
    // av_image_fill_arrays(
    //     bgrFrame.data, bgrFrame.linesize, bgrBuff,
    //     (AVPixelFormat)bgrFrame.format,bgrFrame.width, bgrFrame.height,1);

    // sws_getContext(pViCodecCtx->width, pViCodecCtx->height, pViCodecCtx->pix_fmt,
    //      bgrFrame.width, bgrFrame.height, (AVPixelFormat)bgrFrame.format, SWS_BICUBIC, NULL, NULL, NULL);
         
    

}