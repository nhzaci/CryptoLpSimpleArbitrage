#include "PoolTest.cxx"

#include "ArbitrageStrategy.cxx"

TEST_F(PoolTest, DoNotExecuteUnprofitableTrade) {
  using namespace strategy;

  auto strat = ArbitrageStrategy();

  auto future = strat.calculateAsync(poolA, poolA);
  auto [ethRequired, profit] = future.get();

  EXPECT_DOUBLE_EQ(ethRequired, 0);
  EXPECT_DOUBLE_EQ(profit, 0);
}
