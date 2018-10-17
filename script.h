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

namespace slightAnimation {

    class Clip {
    private:
        string name;
        Mat material;
    public:
        Clip(const string &name, const string &filename) {
            this->material = imread(filename, IMREAD_COLOR);
        }

        const string &getName() const {
            return name;
        }

        void setName(const string &name) {
            Clip::name = name;
        }

        const Mat &getMaterial() const {
            return material;
        }

        void setMaterial(const Mat &material) {
            Clip::material = material;
        }
    };

    class Point {
    private:
        int x, y;
    public:
        Point(int x, int y) : x(x), y(y) {}

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
        Clip clip;
        KeyFrame start;
        KeyFrame stop;
    public:
        class WrongFramesRange {
        };

        class WrongFrameIndex {
        };

        Animation(
                const Clip &clip,
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

        const Point &getPositionFor(unsigned long index) const {
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

            const Point &point = Point(aproxX, aproxY);
            return point;
        }
    };

    class AnimationScript {
    private:
        string m_outputFilename;
        unsigned int m_width;
        unsigned int m_height;
        list <Animation> m_clips;
    };
}

#endif //SLIGHT_ANIMATION_SCRIPT_H
