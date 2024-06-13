# 1 配置时注意添加系统变量

# 2 结构体
## 2.1 AVFormatContext
### iformat/oformat：输入/输出的格式
### pb：I/O上下文，指向用于读写媒体文件的I/O上下文，负责时机的文件I/O操作。
### streams：媒体流数组（AVStream* streams[MAX_STREAMS])。包含文件中所有流（视频、音频、字幕等）。
### duration：媒体文件的时常。以微秒为单位
### start_time: 媒体文件的起始时间（int64_t start_time）。以微秒为单位表示的媒   文件的起始时间。
### bit_rate: 媒体文件的比特率（int64_t bit_rate）。表示媒体文件的平均比特率。
### filename: 媒体文件名（char filename[1024]）。存储打开的媒体文件的文件名。
### metadata: 元数据（AVDictionary *metadata）。指向一个字典，存储了媒体文件的   数据信息，比如标题、作者、日期等。

## 2.2 AVStream：表示媒体流的信息
### index：流的索引，表示在 AVFormatContext 中的流数组中的位置。
### id: 流的唯一标识符，用于识别流。
### codecpar: 指向一个 AVCodecParameters 结构体的指针，包含了流的编解码器参数信息，比如编码格式、分辨率、帧率等。
### time_base: 表示流中时间单位的分数，用于将时间戳转换为时间值。例如，如果 time_base 为 (1, 1000)，则时间戳单位为毫秒。
### start_time: 流的起始时间，以 AV_TIME_BASE 为单位。
### duration: 流的时长，以 AV_TIME_BASE 为单位。
### nb_frames: 流中帧的数量。
### disposition: 表示流的属性，比如是否是主音频流、主视频流、是否包含字幕等。
### metadata: 元数据字典，存储了流的元数据信息，比如标题、语言、编码器名称等。
### avg_frame_rate: 流中平均帧率。
### r_frame_rate: 流中原始帧率。
### codec_info_nb_frames: 编解码器提供的帧数量信息。
### priv_data: 指向流的私有数据，用于存储特定于流的信息。

## 2.3 AVCodec：编解码器的信息
### name：编解码器的名称。这是一个字符串，用于标识编解码器的类型，比如 “h264”、“aac” 等。
### long_name: 编解码器的长名称。提供了更详细的描述，比如 “H.264”、“Advanced Audio Coding” 等。
### type: 编解码器的类型。这是一个枚举值，表示编解码器所处理的媒体类型，比如视频、音频、字幕等。
### id: 编解码器的唯一标识符。这是一个枚举值，表示编解码器的具体类型，比如 H.264、AAC、MP3 等。
### capabilities: 编解码器的能力标志。这个参数指示了编解码器所支持的功能，比如是否支持硬件加速、是否支持多线程编解码等。
### supported_framerates: 编解码器支持的帧率。这是一个指向 AVRational 结构体数组的指针，表示编解码器能够处理的帧率范围。
### supported_samplerates: 编解码器支持的采样率。这是一个整数数组，表示编解码器能够处理的音频采样率范围。
### pix_fmts: 编解码器支持的像素格式。这是一个枚举值数组，表示编解码器能够处理像素格式范围。
### sample_fmts: 编解码器支持的采样格式。这是一个枚举值数组，表示编解码器能够处理的音频采样格式范围。
### priv_class: 指向私有类的指针，用于访问编解码器的私有选项。
### profiles: 编解码器支持的配置文件。这是一个指向 AVProfile 结构体数组的指针，表示编解码器支持的不同配置文件。
### wrapper_name: 编解码器所使用的包装器（wrapper）名称。

## 2.4 AVPacket：表示编码后的数据包
### AVBufferRef *buf: 指向数据包的缓冲区引用。这个参数指示了存储编码数据的缓冲区。
### int64_t pts: 指定数据包的显示时间戳（Presentation Time Stamp）。这是一个以 AV_TIME_BASE 为单位的时间戳，用于指示数据包应该在播放器中显示的时间。
### int64_t dts: 指定数据包的解码时间戳（Decode Time Stamp）。这是一个以 AV_TIME_BASE 为单位的时间戳，用于指示数据包在解码时应该被解码的时间。
### int64_t duration: 数据包的时长。表示数据包的持续时间，以 AV_TIME_BASE 为单位。
### int64_t pos: 数据包在媒体文件中的位置。用于指示数据包在文件中的偏移量，以字节为单位。
### int size: 数据包的大小。表示数据包中编码数据的字节数。
### int stream_index: 数据包所属的流的索引。指示数据包属于 AVFormatContext 中流数组中的哪一个流。
### int flags: 数据包的标志。这个参数包含了一些标志位，表示数据包的一些属性，比如是否为关键帧等。
### AVPacketSideData *side_data: 指向附加数据（Side Data）的指针。这个参数用于存储与数据包相关的额外信息，比如编解码器的配置信息、裁剪信息等。
### int side_data_elems: 附加数据的数量。表示 side_data 数组中的附加数据的数量。
### int duration_count: 数据包的时长计数。用于指示 duration 字段中的时间单位数量。

## 2.5 AVFrame：表示解码后的音视频帧数据
### uint8_t *data[AV_NUM_DATA_POINTERS]: 数据指针数组。这个数组用于存储指向帧数据的指针，每个元素对应一个数据平面。
### int linesize[AV_NUM_DATA_POINTERS]: 数据行大小数组。这个数组存储了每个数据平面的行大小（以字节为单位）。
### uint8_t **extended_data: 扩展数据指针。指向一个指针数组，用于访问帧数据的每个数据平面的指针。
### int width: 帧的宽度（像素数）。
### int height: 帧的高度（像素数）。
### int format: 像素格式。表示帧数据的像素格式，如 YUV420、RGB24 等。
### int64_t pts: 显示时间戳（Presentation Time Stamp）。指示帧应该在播放器中显示的时间。
### int64_t pkt_pts: 数据包的显示时间戳。指示生成该帧的数据包的显示时间戳。
### int64_t pkt_dts: 数据包的解码时间戳。指示生成该帧的数据包的解码时间戳。
### key_frame: 关键帧标志。指示该帧是否为关键帧。
### AVRational sample_aspect_ratio: 采样纵横比。表示帧的采样纵横比，用于指示像素的宽高比。
### void *opaque: 不透明数据。用于存储一些不透明的私有数据。
### int quality: 图像质量。表示帧的图像质量，一般用于视频编码。
### int coded_picture_number: 编码图像编号。指示编码器为该帧分配的编码图像编号
### int display_picture_number: 显示图像编号。指示编码器为该帧分配的显示图像编号。


# 3 音视频编解码器
## 3.1 音频
### AAC
### MP3
### AC-3/E-AC-3
### PCM
### ...

## 3.2 视频
### H.264/AVC
### H.265/HEVC
### MPEG-2
### ...

# 4 AvFrame对应的音视频格式

## 4.1 视频帧数据格式

### 1.YUV格式
   YUV420P：最常见的YUV格式之一，包括亮度(Y)和色度(U、V)三个平面，其中色度平面的采样率是亮度平面的四分之一。
   YUV422P：与 YUV420P 类似，但色度平面的采样率是亮度平面的一半。
   YUV444P：每个像素的亮度和色度分量都有独立的采样点，没有颜色抽样。
   YUV410P、YUV411P：在垂直和水平方向上进行更多的颜色抽样，减少了色度分量的存储。
### 2.RGB格式
   RGB24：每个像素使用 24 位（3 字节）表示，依次是红、绿、蓝三个分量，存储顺序通常是从左上角开始，从左到右、从上到下依次排列。
   RGB32：与 RGB24 类似，但每个像素使用 32 位（4 字节）表示，通常最后一个字节用于填充 alpha 通道。
### 3.灰度图像
   GRAY8：灰度图像，每个像素只有一个亮度分量，使用 8 位表示。
### 4.其他格式
   BGR、BGRA：与 RGB 类似，但存储顺序颠倒，即蓝色分量在前。
   ARGB、ABGR：与 RGB 类似，但带有 alpha 通道。

## 4.2 视频帧数据格式
### 1.PCM（脉冲编码调制）
   PCM 是最常见的音频数据格式之一，它直接表示原始音频采样值。PCM 格式的音频通常以帧为单位进行处理，每个帧包含多个采样点。
   常见的PCM格式包括：
      AV_SAMPLE_FMT_U8：无符号 8 位整数。
      AV_SAMPLE_FMT_S16：有符号 16 位整数。
      AV_SAMPLE_FMT_S32：有符号 32 位整数。
      AV_SAMPLE_FMT_FLT：单精度浮点数。
      AV_SAMPLE_FMT_DBL：双精度浮点数。
### 2.压缩格式
   AAC（Advanced Audio Coding）、MP3（MPEG-1/2 Audio Layer III）、Opus 等是常见的音频压缩格式，它们采用了各种压缩算法来减小音频数据的大小。
   这些格式通常在传输和存储时使用，可以减小带宽和存储空间的占用。