#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "script.h"

using namespace std;
using namespace cv;
namespace pt = boost::property_tree;
using namespace SlightAnimation;

KeyFrame parseKeyFrame(const pt::ptree &node) {
    const unsigned long &frameNumber = atoi(node.get_child("frame").data().c_str());
//    cout << frameNumber << endl;
    const pt::ptree &posNode = node.get_child("position");
    const int &posX = atoi(posNode.get_child("x").data().c_str());
    const int &posY = atoi(posNode.get_child("y").data().c_str());

    return KeyFrame(frameNumber, SlightAnimation::Point(posX, posY));
}

int main(int argc, char **argv) {
    if (2 != argc) {
        cerr << "Usage: \nslight-animation <script filename>" << endl;
        return -1;
    }

    std::string scriptFilename = argv[1];

    pt::ptree root;
    pt::read_json(scriptFilename, root);

//    int width = root.get<int>("width", 0);
//    int height = root.get<int>("height", 0);
    string outputFilename = root.get<char *>("output-file", 0);

    AnimationScript *animationScript = new AnimationScript();
    animationScript->setWidth(root.get<unsigned int>("width", 0));
    animationScript->setHeight(root.get<unsigned int>("height", 0));

    for (pt::ptree::value_type &clipPair : root.get_child("clips")) {
        string &filename = clipPair.second.data();
        Mat m = imread(filename);
        const string &name = clipPair.first.data();
        Clip c = Clip(name, &m);
        animationScript->addClip(c);
    }

    for (pt::ptree::value_type &animation : root.get_child("animations")) {
        string &clipName = animation.second.get_child("clip").data();
        const Clip *clip = animationScript->getClip(clipName);

        const pt::ptree &start = animation.second.get_child("start");
        const KeyFrame &startFrame = parseKeyFrame(start);

        const auto &stop = animation.second.get_child("stop");
        const KeyFrame &stopFrame = parseKeyFrame(stop);

        animationScript->addAnimation(Animation(clip, startFrame, stopFrame));
    }

    VideoWriter video(outputFilename, CV_FOURCC('M', 'J', 'P', 'G'), 10,
            Size(animationScript->getWidth(), animationScript->getHeight()), true);
    for (unsigned long i = 0; i < 128; ++i) {
        const Mat &frame = animationScript->getFrame(i);
        video.write(frame);
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