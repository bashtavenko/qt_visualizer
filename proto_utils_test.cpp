#include "proto_utils.h"
#include <absl/status/status_matchers.h>
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

namespace qt_visualizer {
namespace {

using ::absl_testing::IsOkAndHolds;
using ::absl_testing::IsOk;
using ::testing::HasSubstr;

TEST(GetTextFromFile, Works) {
  EXPECT_THAT(GetTextFromFile("../testdata/lidar.txtpb"),
              IsOkAndHolds(HasSubstr("theta: 71")));
}

TEST(ParseProtoFromText, Works) {
  auto text_proto = GetTextFromFile("../testdata/lidar.txtpb");
  EXPECT_THAT(text_proto, IsOk());
  auto proto = ParseProtoFromText(text_proto.value());
  EXPECT_THAT(text_proto, IsOk());
}

}  // namespace
}  // namespace qt_visualizer