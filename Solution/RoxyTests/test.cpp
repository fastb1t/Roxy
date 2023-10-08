#include "pch.h"

#include "../Roxy/Roxy/Algorithms.h"

//TEST(TestCaseName, TestName) {
//  EXPECT_EQ(1, 1);
//  EXPECT_TRUE(true);
//}

TEST(RoxyAlgorithms_TextHasNumberA, Test1) {
    EXPECT_EQ(1, Roxy::TextHasNumberA("1"));
    EXPECT_TRUE(true);
}

TEST(RoxyAlgorithms_TextHasNumberA, Test2) {
    EXPECT_EQ(1, Roxy::TextHasNumberA("0"));
    EXPECT_TRUE(true);
}

TEST(RoxyAlgorithms_TextHasNumberA, Test3) {
    EXPECT_EQ(1, Roxy::TextHasNumberA("-1"));
    EXPECT_TRUE(true);
}

TEST(RoxyAlgorithms_TextHasNumberA, Test4) {
    EXPECT_EQ(0, Roxy::TextHasNumberA("a"));
    EXPECT_TRUE(true);
}
