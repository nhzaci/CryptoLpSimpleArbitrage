#include <gtest/gtest.h>

#include "ArbitrageStrategyTest.cxx"
#include "PoolTest.cxx"
#include "TokenTest.cxx"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
