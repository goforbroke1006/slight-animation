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

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif //SLIGHT_ANIMATION_SCRIPT_TESTS_H
