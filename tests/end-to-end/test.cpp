#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "TestUtils.hpp"

std::pair<std::string, std::string> get(const std::string& str) {
  auto curPath =
      std::filesystem::current_path().generic_string() +
      std::filesystem::path("/tests/end-to-end/data/").generic_string();
  auto in = curPath + str;
  auto correct = curPath + str + "_correct";
  return {in, correct};
}

std::pair<std::string, std::string> test(const std::string& in) {
  auto [inPath, expectPath] = get(in);
  testing::internal::CaptureStdout();
  run(inPath);
  return {testing::internal::GetCapturedStdout(), readFile(expectPath)};
}

TEST(EndToEnd, example) {
  const auto& [real, expected] = test("example");
  EXPECT_EQ(real, expected);
}

TEST(EndToEnd, all) {
  const auto& [real, expected] = test("all");
  EXPECT_EQ(real, expected);
}

TEST(EndToEnd, eventErrNotOpenYet) {
  const auto& [real, expected] = test("eventErrNotOpenYet");
  EXPECT_EQ(real, expected);
}

TEST(EndToEnd, eventErrYouShallNotPass) {
  const auto& [real, expected] = test("eventErrYouShallNotPass");
  EXPECT_EQ(real, expected);
}

TEST(EndToEnd, eventErrPlaceIsBusy) {
  const auto& [real, expected] = test("eventErrPlaceIsBusy");
  EXPECT_EQ(real, expected);
}

TEST(EndToEnd, eventErrClientUnknown) {
  const auto& [real, expected] = test("eventErrClientUnknown");
  EXPECT_EQ(real, expected);
}

TEST(EndToEnd, ICanWaitNoLonger) {
  const auto& [real, expected] = test("eventErrICanWaitNoLonger");
  EXPECT_EQ(real, expected);
}

TEST(EndToEnd, parseErrFormat) {
  const auto& [real, expected] = test("parseErrFormat");
  EXPECT_EQ(real, expected);
}

TEST(EndToEnd, parseErrTabelN) {
  const auto& [real, expected] = test("parseErrTabelN");
  EXPECT_EQ(real, expected);
}

TEST(EndToEnd, parseErrClientName) {
  const auto& [real, expected] = test("parseErrClientName");
  EXPECT_EQ(real, expected);
}

TEST(EndToEnd, parseErrEventOrder) {
  const auto& [real, expected] = test("parseErrEventOrder");
  EXPECT_EQ(real, expected);
}

TEST(EndToEnd, parseErrTime) {
  const auto& [real, expected] = test("parseErrTime");
  EXPECT_EQ(real, expected);
}

TEST(EndToEnd, parseEmptyEvent) {
  const auto& [real, expected] = test("parseEmptyEvent");
  EXPECT_EQ(real, expected);
}

TEST(EndToEnd, checkOrderLeft) {
  const auto& [real, expected] = test("checkOrderLeft");
  EXPECT_EQ(real, expected);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
