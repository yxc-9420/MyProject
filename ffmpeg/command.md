# 添加水印
    ffmpeg -ss 00:00:00 -t 30  -i live.mp4 -vf "movie=cat1.JPG[watermark];[in][watermark]overlay=enable='gt(t,5)':x=10+t * 10:y=10[out1];[out1]drawtext=fontfile=simhei.ttf: text= 'xiaomiao' :x=W-text_w-10:y=10:fontsize=48:fontcolor=white:shadowy=2[out]" -shortest -y output.mp4

    -ss 00:00:00 -t 30  放在-i前面，从开始截取30秒
    -i live.mp4         输入文件live.mp4
    -vf "..."           添加滤镜，也可以使用 -filter-complex
    movie=cat1.JPG[watermark]     表示输入图片并输出为watermark流
    [in][watermark]overlay=enable='gt(t,5)':x=10+t * 10:y=10[out1]   表示将输入流和watermark流作为两个输入，使用overlay将watermark流指定在5秒钟之后开始显示，并添加到指定位置,，并输出为out1流
    [out1]drawtext=fontfile=simhei.ttf: text= 'xiaomiao' :x=W-text_w-10:y=10:fontsize=48:fontcolor=white:shadowy=2[out]  表示输入流为out1，在out1流上添加文字水印。（W：表示视频宽度，text_w表示文字宽度，并输出为out流，[out]可以省略

# 主要选项
## 1、-f fmt (input/output) 
    指定输入或者输出文件格式。常规可省略而使用依据扩展名的自动指定，但一些选项需要强制明确设定。
## 2、-i filename （input） 
    指定输入文件。
## 3、-y （global） 
    默认自动覆盖输出文件，而不再询问确认。
## 4、-n （global） 
    不覆盖输出文件，如果输出文件已经存在则立即退出。
## 5、-t duration（input/output） 
    限制输入/输出的时间。如果是在 -i 前面，就是限定从输入中读取多少时间的数据；如果是用于限定输出文件，则表示写入多少时间数据后就停止。duration可以是以秒为单位的数值或者 hh:mm:ss[.xxx] 格式的时间值。 注意 -to 和 -t 是互斥的，-t 有更高优先级。
## 6、-to position (output) 
    只写入position时间后就停止，position可以是以秒为单位的数值或者 hh:mm:ss[.xxx]格式的时间值。 注意 -to 和 -t 是互斥的，-t 有更高优先级。
## 7、-ss position (input/output)
    当在 -i 前，表示定位输入文件到position指定的位置。注意可能一些格式是不支持精确定位的，所以ffmpeg可能是定位到最接近position（在之前）的可定位点。position可以是以秒为单位的数值或者 hh:mm:ss[.xxx] 格式的时间值。
## 8、-itsoffset offset (input) 
    设置输入文件的时间偏移。offset 必须采用时间持续的方式指定，即可以有-号的时间值（以秒为单位的数值或者 hh:mm:ss[.xxx] 格式的时间值）。偏移会附加到输入文件的时间码上，意味着所指定的流会以时间码+偏移量作为最终输出时间码。
## 9、-timestamp date (output) 
    设置在容器中记录时间戳。
## 10、-codec[:stream_specifier] 
    codec (input/output,per-stream) 为特定的文件选择编/解码模式，对于输出文件就是编码器，对于输入或者某个流就是解码器。选项参数中 codec 是编解码器的名字，或者是 copy（仅对输出文件）则意味着流数据直接复制而不再编码。

# 视频选项
## 1、-vframes number (output) 
    设置输出文件的帧数，是 -frames:v 的别名。
## 2、-r[:stream_specifier] fps (input/output,per-stream) 
    设置帧率（一种Hz值，缩写或者分数值）。
## 3、-s[:stream_specifier] size (input/output,per-stream) 
    设置帧的尺寸。
## 4、-vn (output) 
    禁止输出视频。
## 5、-vcodec codec (output) 
    设置视频编码器，这是 -codec:v 的一个别名。
## 6、aspect[:stream_specifier] aspect (output,per-stream) 
    指定视频的纵横比（长宽显示比例）。aspect 是一个浮点数字符串或者num:den格式字符串(其值就是num/den)，例如"4:3","16:9","1.3333"以及"1.7777"都是常用参数值。

# 音频选项
## 1、-aframes number (output) 
    设置 number 音频帧输出，是 -frames:a 的别名。
## 2、-ar[:stream_specifier] freq (input/output,per-stream) 
    设置音频采样率。默认是输出同于输入。对于输入进行设置，仅仅通道是真实的设备或者raw数据分离出并映射的通道才有效。对于输出则可以强制设置音频量化的采用率。
## 3、-aq q (output) 
    设置音频品质(编码指定为VBR)，它是 -q:a 的别名。
## 4、-ac[:stream_specifier] channels (input/output,per-stream) 
    设置音频通道数。默认输出会有输入相同的音频通道。对于输入进行设置，仅仅通道是真实的设备或者raw数据分离出并映射的通道才有效。
## 5、-an (output) 
    禁止输出音频。
## 6、-acode codec (input/output) 
    设置音频解码/编码的编/解码器，是 -codec:a 的别名。

# 字幕选项
## 1、-scodec codec（input/output） 
    设置字幕解码器，是 -codec:s 的别名。
## 2、-sn (output) 
    禁止输出字幕。
## 3、-fix_sub_duration 
    修正字幕持续时间。
## 4、-canvas_size size 
    设置字幕渲染区域的尺寸（位置）。