#ifndef BXC_LOG_H
#define BXC_LOG_H
#include <time.h>
#include <string>
#include <chrono>

static int64_t getCurTimestamp()// 获取毫秒级时间戳（13位）
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).
        count();

}
static int getCurTimestamp_second()// 获取秒级时间戳（10位）
{
    int64_t curTimestamp = getCurTimestamp();

    return int(curTimestamp / 1000);
}
static std::string getTime() {
    const char* time_fmt = "%Y-%m-%d %H:%M:%S";
    time_t t = time(nullptr);
    char time_str[64];
    strftime(time_str, sizeof(time_str), time_fmt, localtime(&t));

    return time_str;
}
#define LOGI(format, ...)  fprintf(stderr,"[I]%s [%d %s()] " format "\n", getTime().data(),__LINE__,__func__ ,##__VA_ARGS__)
#define LOGE(format, ...)  fprintf(stderr,"[E]%s [%d %s()] " format "\n",getTime().data(),__LINE__,__func__ ,##__VA_ARGS__)

#endif //BXC_LOG_H