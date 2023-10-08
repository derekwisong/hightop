#include <gtest/gtest.h>

#include <hightop/procs.h>

TEST(Processes, CountProcess) {
  EXPECT_GT(hightop::count_processes(), 0) << "Expected at least 1 process to be running";
}