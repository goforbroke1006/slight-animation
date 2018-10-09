#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char **argv) {

    std::string assetsDir = "/home/goforbroke/CLionProjects/slight-animation/assets/";

    VideoWriter video("out.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(400, 300), true);
    for (int i = 0; i < 100; ++i) {
        Mat dst = imread(assetsDir + "sample-1.jpg", IMREAD_COLOR);

        Mat src2;
        if (i > 10) {
            src2 = imread(assetsDir + "1.png", CV_LOAD_IMAGE_COLOR);
            addWeighted(dst, 1.0, src2, 1.0, 0.0, dst);
        }
        if (i > 30) {
            src2 = imread(assetsDir + "2.png", CV_LOAD_IMAGE_COLOR);
            addWeighted(dst, 1.0, src2, 1.0, 0.0, dst);
        }
        if (i > 50) {
            src2 = imread(assetsDir + "3.png", CV_LOAD_IMAGE_COLOR);
            addWeighted(dst, 1.0, src2, 1.0, 0.0, dst);
        }
        if (i > 70) {
            src2 = imread(assetsDir + "4.png", CV_LOAD_IMAGE_COLOR);
            addWeighted(dst, 1.0, src2, 1.0, 0.0, dst);
        }

        //dst.convertTo(dst, CV_8UC1);
        video.write(dst);
    }
    return 0;
}