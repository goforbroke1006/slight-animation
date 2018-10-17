//
// Created by goforbroke on 16.10.18.
//

#ifndef SLIGHT_ANIMATION_SCRIPT_TESTS_H
#define SLIGHT_ANIMATION_SCRIPT_TESTS_H

#include <gtest/gtest.h>
#include "../script.h"

using namespace slightAnimation;

TEST(SquareRootTest, PositiveNos) {

    const Clip clip = Clip("", "");
    const KeyFrame start = KeyFrame(100, slightAnimation::Point(300, 400));
    const KeyFrame stop = KeyFrame(200, slightAnimation::Point(100, 800));
    unique_ptr<Animation> animation(
            new Animation(clip, start, stop)
    );

    const slightAnimation::Point &tmpPoint = animation->getPositionFor(175);

    const int x = tmpPoint.getX();
    const int y = tmpPoint.getY();

    ASSERT_EQ(150, x);
    ASSERT_EQ(700, y);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif //SLIGHT_ANIMATION_SCRIPT_TESTS_H
