#include <gtest/gtest.h>

#include <string>
using std::string;

const char *actualValTrue = "hello gtest";
const char *expectVal = "hello gtest";

TEST(StrCompare, CStrEqual) { EXPECT_STREQ(expectVal, actualValTrue); }
