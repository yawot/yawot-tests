#include "gtest/gtest.h"

#include "yawot/version.h"

TEST(Version, version_match) {
    unsigned int version = yawot::getVersion();
    EXPECT_EQ(0x000100, version);
}

