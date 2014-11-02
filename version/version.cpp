#include "gtest/gtest.h"

#include "yawot/version.h"

TEST(Version, version_match) {
    unsigned int version = yawot::version();
    EXPECT_EQ(0x000100, version);

    std::string version_string = yawot::versionString();
    EXPECT_EQ("0.1.0", version_string);
}

