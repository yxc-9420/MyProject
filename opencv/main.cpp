#include<bits/stdc++.h>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core_c.h>
#include<opencv2/core/types_c.h>
#include<opencv2/core/mat.hpp>

using namespace cv;
using namespace std;

/// @brief 显示图片
void test1()
{
    const cv::String file = "D:/Personal/repos/MyProject/resource/cat1.jpg";
    cv::Mat img = cv::imread(file, cv::IMREAD_COLOR);
    cv::namedWindow("cat",cv::WINDOW_KEEPRATIO);
    cv::imshow("cat",img);
}

void test2()
{
    cv::Size size(500,500);
    cv::Mat img = cv::Mat::ones(size,USAGE_DEFAULT) * 255;
    cv::line(img, {0,0}, {300,300}, {0});
    cv::imshow("test2",img);
}

int main()
{
    test1();
    test2();
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}