#include <gtest/gtest.h>

#include "../src/object.h"

TEST(ObjectTest, TestHashKeys) {
    auto str1 = std::make_shared<String>("some string");
    auto str2 = std::make_shared<String>("some string");
    auto str3 = std::make_shared<String>("another one");
    auto str4 = std::make_shared<String>("another one");

    EXPECT_EQ(str1->hashKey().value, str2->hashKey().value);
    EXPECT_EQ(str3->hashKey().value, str4->hashKey().value);
    EXPECT_NE(str1->hashKey().value, str3->hashKey().value);
}
