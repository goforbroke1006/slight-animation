#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
using namespace cv;
namespace pt = boost::property_tree;

int main(int argc, char **argv) {
    if (2 != argc) {
        cerr << "Usage: \nslight-animation <script filename>" << endl;
        return -1;
    }

    std::string scriptFilename = argv[1];

    pt::ptree root;
    pt::read_json(scriptFilename, root);

    int width = root.get<int>("width", 0);
    int height = root.get<int>("height", 0);
    string outputFilename = root.get<char *>("output-file", 0);

    VideoWriter video(outputFilename, CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(width, height), true);

    for (pt::ptree::value_type &animation : root.get_child("animations")) {
        cout << animation.second.get_child("clip").data() << endl;

        cout << animation.second.get_child("start").get_child("frame").data() << endl;
        cout << animation.second.get_child("start").get_child("position").get_child("x").data() << endl;
        cout << animation.second.get_child("start").get_child("position").get_child("y").data() << endl;

        cout << animation.second.get_child("stop").get_child("frame").data() << endl;
        cout << animation.second.get_child("stop").get_child("position").get_child("x").data() << endl;
        cout << animation.second.get_child("stop").get_child("position").get_child("y").data() << endl;

        cout << endl;
    }

    /*for (int i = 0; i < 100; ++i) {
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
        video.write(dst);
    }*/

    return 0;
}