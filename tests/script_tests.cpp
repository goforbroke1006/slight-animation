//
// Created by goforbroke on 16.10.18.
//

#ifndef SLIGHT_ANIMATION_SCRIPT_TESTS_H
#define SLIGHT_ANIMATION_SCRIPT_TESTS_H

#include <gtest/gtest.h>
#include "../script.h"

using namespace SlightAnimation;

TEST(AnimationTest, Calcualtion) {
    const Clip *clip = new Clip("", nullptr);
    const KeyFrame start = KeyFrame(100, SlightAnimation::Point(300, 400));
    const KeyFrame stop = KeyFrame(200, SlightAnimation::Point(100, 800));
    unique_ptr<Animation> animation(
            new Animation(clip, start, stop)
    );

    const SlightAnimation::Point &tmpPoint = animation->getPositionFor(175);

    const int x = tmpPoint.getX();
    const int y = tmpPoint.getY();

    ASSERT_EQ(150, x);
    ASSERT_EQ(700, y);
}

TEST(AnimationTest, Idle) {
    const Clip *clip = new Clip("", nullptr);
    const KeyFrame start = KeyFrame(100, SlightAnimation::Point(200, 400));
    const KeyFrame stop = KeyFrame(200, SlightAnimation::Point(200, 400));
    unique_ptr<Animation> animation(
            new Animation(clip, start, stop)
    );

    const SlightAnimation::Point &tmpPoint = animation->getPositionFor(134);

    const int x = tmpPoint.getX();
    const int y = tmpPoint.getY();

    ASSERT_EQ(200, x);
    ASSERT_EQ(400, y);
}

TEST(AnimationTest, Idle2) {
    const Clip *clip = new Clip("", nullptr);
    const KeyFrame start = KeyFrame(100, SlightAnimation::Point(0, 0));
    const KeyFrame stop = KeyFrame(200, SlightAnimation::Point(0, 0));
    unique_ptr<Animation> animation(
            new Animation(clip, start, stop)
    );

    const SlightAnimation::Point &tmpPoint = animation->getPositionFor(134);

    const int x = tmpPoint.getX();
    const int y = tmpPoint.getY();

    ASSERT_EQ(0, x);
    ASSERT_EQ(0, y);
}

TEST(GetVisibleAreaForGraphics, PositiveTest) {
    Mat material = Mat::zeros(Size(400, 300), CV_8U);
    Size canvasSize = Size(400, 300);
    SlightAnimation::Point point = SlightAnimation::Point(100, 100);
    const Rect &actual = SlightAnimation::getVisiblePartRect(material, canvasSize, point);

    const int x = actual.x;
    const int w = actual.width;

    ASSERT_EQ(0, x);
    ASSERT_EQ(300, w);
}

TEST(GetVisibleAreaForGraphics, PositiveTest2) {
    Mat material = Mat::zeros(Size(400, 300), CV_8U);
    Size canvasSize = Size(400, 300);
    SlightAnimation::Point point = SlightAnimation::Point(-100, -150);
    const Rect &actual = SlightAnimation::getVisiblePartRect(material, canvasSize, point);

    const int x = actual.x;
    const int w = actual.width;
    const int y = actual.y;

    ASSERT_EQ(100, x);
    ASSERT_EQ(300, w);
    ASSERT_EQ(150, y);
}

TEST(GetVisibleAreaForGraphics, PositiveTest3) {
    Mat material = Mat::zeros(Size(400, 300), CV_8U);
    Size canvasSize = Size(400, 300);
    SlightAnimation::Point point = SlightAnimation::Point(-400, -400);

    try {
        const Rect &actual = SlightAnimation::getVisiblePartRect(material, canvasSize, point);
        FAIL() << "Expected SlightAnimation::out_of_range";
    } catch (SlightAnimation::HasNoVisiblePartException &ex) {
        ASSERT_GT(strlen(ex.what()), 0);
    }
}


TEST(getCanvasEmbeddingArea_check, PositiveTest1) {
    Mat material = Mat::zeros(Size(100, 60), CV_8U);
    Size canvasSize = Size(400, 300);
    SlightAnimation::Point point = SlightAnimation::Point(150, 100);
    const Rect &actual = SlightAnimation::getCanvasEmbeddingArea(material, canvasSize, point);

    const int x = actual.x;
    const int w = actual.width;
    const int y = actual.y;
    const int h = actual.height;

    ASSERT_EQ(150, x);
    ASSERT_EQ(100, y);
    ASSERT_EQ(100, w);
    ASSERT_EQ(60, h);
}

TEST(getCanvasEmbeddingArea_check, PositiveTest2_invisiblePart) {
    Mat material = Mat::zeros(Size(100, 60), CV_8U);
    Size canvasSize = Size(400, 300);
    SlightAnimation::Point point = SlightAnimation::Point(-50, 100);
    const Rect &actual = SlightAnimation::getCanvasEmbeddingArea(material, canvasSize, point);

    const int x = actual.x;
    const int w = actual.width;
    const int y = actual.y;
    const int h = actual.height;

    ASSERT_EQ(0, x);
    ASSERT_EQ(100, y);
    ASSERT_EQ(50, w);
    ASSERT_EQ(60, h);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif //SLIGHT_ANIMATION_SCRIPT_TESTS_H
