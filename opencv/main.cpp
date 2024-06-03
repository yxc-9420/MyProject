#include<bits/stdc++.h>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core_c.h>
#include<opencv2/core/types_c.h>
#include<opencv2/core/mat.hpp>

using namespace cv;
using namespace std;

int main()
{
    const cv::String file = "cat1.jpg";
    cv::Mat img = cv::imread(file, cv::IMREAD_COLOR);
    cv::namedWindow("cat",cv::WINDOW_KEEPRATIO);
    cv::imshow("cat",img);
    cv::waitKey(0);
    cv::destroyWindow("cat");
    return 0;
}