#include "gtest/gtest.h"

int main(int argc, char **argv) {
    // return 23;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
