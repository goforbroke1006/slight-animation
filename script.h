//
// Created by goforbroke on 12.10.18.
//

#ifndef SLIGHT_ANIMATION_SCRIPT_H
#define SLIGHT_ANIMATION_SCRIPT_H

#include <string>
#include <list>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace SlightAnimation {

    int clamp(int val, int min, int max) {
        if (val < min) return min;
        if (val > max) return max;
        return val;
    }

    class Point {
    private:
        int x, y;
    public:
        Point(int x, int y)
                : x(x),
                  y(y) {}

        const int &getX() const {
            return x;
        }

        void setX(int x) {
            Point::x = x;
        }

        const int &getY() const {
            return y;
        }

        void setY(int y) {
            Point::y = y;
        }
    };

    class HasNoVisiblePartException : public exception {
    public:
        const char *what() const throw() {
            return "Mat has no visible part!";
        }
    };

    Rect getVisiblePartRect(Mat material, Size canvasSize, SlightAnimation::Point startPosition) {
        int x = 0;
        if (startPosition.getX() < 0)
            x -= startPosition.getX();

        int y = 0;
        if (startPosition.getY() < 0)
            y -= startPosition.getY();

        int w = material.cols;
        if (startPosition.getX() < 0)
            w -= abs(startPosition.getX());
        if (startPosition.getX() + w > canvasSize.width)
            w -= (startPosition.getX() + w - canvasSize.width);

        int h = material.rows;
        if (startPosition.getY() < 0)
            h -= abs(startPosition.getY());
        if (startPosition.getY() + h > canvasSize.height)
            h -= (startPosition.getY() + h - canvasSize.height);

        if (
                x >= material.cols
                || y >= material.rows
                || w <= 0
                || h <= 0
                )
            throw HasNoVisiblePartException();

        return Rect(x, y, w, h);
    }

    Rect getCanvasEmbeddingArea(Mat material, Size canvasSize, SlightAnimation::Point startPosition) {
        return Rect(); // TODO: implement me!!!
    }

    class Clip {
    private:
        string name;
        const Mat *material;
    public:
        Clip(const string &name, const Mat *material)
                : name(name),
                  material(material) {
        }

        const string &getName() const {
            return name;
        }

        void setName(const string &name) {
            Clip::name = name;
        }

        const Mat *getMaterial() const {
            return material;
        }

        void setMaterial(const Mat *material) {
            Clip::material = material;
        }
    };


    class KeyFrame {
    private:
        unsigned long index;
        Point position;
    public:
        KeyFrame(unsigned long index, const Point &position)
                : index(index),
                  position(position) {}

        unsigned long getIndex() const {
            return index;
        }

        void setIndex(unsigned long index) {
            KeyFrame::index = index;
        }

        const Point &getPosition() const {
            return position;
        }

        void setPosition(const Point &position) {
            KeyFrame::position = position;
        }
    };

    class Animation {
    private:
        const Clip *clip;
        KeyFrame start;
        KeyFrame stop;
    public:
        class WrongFramesRange {
        };

        class WrongFrameIndex {
        };

        Animation(
                const Clip *clip,
                const KeyFrame &start,
                const KeyFrame &stop
        )
                : clip(clip),
                  start(start),
                  stop(stop) {
            if (start.getIndex() >= stop.getIndex()) {
                throw WrongFramesRange();
            }
        }

        const Clip *getClip() const {
            return clip;
        }

        void setClip(const Clip *clip) {
            Animation::clip = clip;
        }

        const KeyFrame &getStart() const {
            return start;
        }

        void setStart(const KeyFrame &start) {
            Animation::start = start;
        }

        const KeyFrame &getStop() const {
            return stop;
        }

        void setStop(const KeyFrame &stop) {
            Animation::stop = stop;
        }

        const Point getPositionFor(unsigned long index) const {
            if (index < start.getIndex() || stop.getIndex() < index) {
                throw WrongFrameIndex();
            }
            int localIndex = (int) (index - start.getIndex());
            int range = (int) (stop.getIndex() - start.getIndex());

            int movementX = stop.getPosition().getX() - start.getPosition().getX();
            int partX = movementX / range;
            int aproxX = start.getPosition().getX()
                         + partX * localIndex;

            int movementY = stop.getPosition().getY() - start.getPosition().getY();
            int partY = movementY / range;
            int aproxY = start.getPosition().getY()
                         + partY * localIndex;

            return Point(aproxX, aproxY);
        }
    };

    class AnimationScript {
    private:
        string outputFilename;
        unsigned int width;
        unsigned int height;
        list <Clip> clips;
        list <Animation> animations;
    public:
        const string &getOutputFilename() const {
            return outputFilename;
        }

        void setOutputFilename(const string &outputFilename) {
            AnimationScript::outputFilename = outputFilename;
        }

        unsigned int getWidth() const {
            return width;
        }

        void setWidth(unsigned int width) {
            AnimationScript::width = width;
        }

        unsigned int getHeight() const {
            return height;
        }

        void setHeight(unsigned int height) {
            AnimationScript::height = height;
        }

        const list <Clip> &getClips() const {
            return clips;
        }

        void addClip(const Clip clip) {
            clips.push_back(clip);
        }

        void setClips(const list <Clip> &clips) {
            AnimationScript::clips = clips;
        }

        const Clip *getClip(const string &name) {
            for (const auto &c : clips) {
                if (c.getName() == name) {
                    return &c;
                }
            }
            return nullptr;
        }

        const list <Animation> &getAnimations() const {
            return animations;
        }

        void setAnimations(const list <Animation> &animations) {
            AnimationScript::animations = animations;
        }

        void addAnimation(const Animation &animation) {
            animations.push_back(animation);
        }

        const Mat getFrame(unsigned long frameIndex) {
//            cv::Rect roi = cv::Rect(0, 0, this->getWidth(), this->getHeight());
//
//            Mat bgMat = Mat::zeros(Size(this->getWidth(), this->getHeight()), CV_8U);
//            cv::Mat out_image = bgMat.clone();
//
//            cv::Mat A_roi = bgMat(roi);
//            cv::Mat out_image_roi = out_image(roi);

            //cv::Mat out_image_roi;

            Mat canvas = Mat::zeros(Size(this->getWidth(), this->getHeight()), CV_8U);

            for (const auto &anim : this->getAnimations()) {
                try {
                    const Point &point = anim.getPositionFor(frameIndex);
                    Mat graphics = *(anim.getClip()->getMaterial());
                    Rect layoutVisiblePart = getVisiblePartRect(graphics, canvas.size(), point);

//                    const Size &size = bgMat.size();
//                    int type = bgMat.type();
//                    cv::Mat out = cv::Mat::zeros(size, type);
//
//                    const int &x = point.getX() < 0 ? 0 : point.getX();
//                    const int &y = point.getY() < 0 ? 0 : point.getY();
//                    int w = (graphics.cols > this->getWidth()) ? graphics.cols - point.getX() : graphics.cols;
//                    int h = bgMat.rows - point.getY();

                    graphics(layoutVisiblePart)
                            .copyTo(canvas(cv::Rect(x, y, w, h)));
//                    cv::addWeighted(A_roi, 1.0, out, 1.0, 0.0, out_image_roi);



                } catch (Animation::WrongFrameIndex ex) {
                    //
                }
            }

            return canvas;
        }
    };
}

#endif //SLIGHT_ANIMATION_SCRIPT_H
