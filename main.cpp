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

    auto *animationScript = new AnimationScript();
    animationScript->setOutputFilename(root.get<char *>("output-file", 0));
    animationScript->setWidth(root.get<unsigned int>("width", 0));
    animationScript->setHeight(root.get<unsigned int>("height", 0));

    for (pt::ptree::value_type &clipPair : root.get_child("clips")) {
        string &filename = clipPair.second.data();
        Mat m = imread(filename);

        if (m.cols == 0 && m.rows == 0) {
            cerr << "Can't find image file: " << filename << endl;
            continue;
        }

        const string &name = clipPair.first;
        Mat *material = new Mat();
        m.copyTo(*material);
        Clip c = Clip(name, material);
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

    unsigned int width = animationScript->getWidth();
    unsigned int height = animationScript->getHeight();
    VideoWriter video(
            animationScript->getOutputFilename(),
            CV_FOURCC('M', 'J', 'P', 'G'),
            12,
            Size(width, height),
            true
    );
    for (unsigned long i = 0; i < 128; ++i) {
        const Mat &frame = animationScript->getFrame(i);
        video.write(frame);
    }

    return 0;
}