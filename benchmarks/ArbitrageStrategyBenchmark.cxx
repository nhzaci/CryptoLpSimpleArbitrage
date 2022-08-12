#include <benchmark/benchmark.h>

#include "ArbitrageStrategy.hxx"
#include "Pool.hxx"
#include "Token.hxx"

class ArbitrageStrategyFixture : public benchmark::Fixture {
public:
  void SetUp(const ::benchmark::State &state) {}
  void TearDown(const ::benchmark::State &state) {}
};

BENCHMARK_DEFINE_F(ArbitrageStrategyFixture, BasicStrategyAsync)
(benchmark::State &st) {
  using namespace dex;
  using namespace strategy;

  static auto strat = ArbitrageStrategy();

  auto poolA =
      Pool(Token::Symbol::LP_A, 1000, Token(Token::Symbol::DAI, 1000000000),
           Token(Token::Symbol::ETH, 4500000));
  auto poolB =
      Pool(Token::Symbol::LP_B, 1000, Token(Token::Symbol::DAI, 1100000000),
           Token(Token::Symbol::ETH, 3000000));
  for (auto _ : st) {
    st.PauseTiming();
    auto future = strat.calculateAsync(poolA, poolB);
    st.ResumeTiming();
    future.get();
  }
}

BENCHMARK_DEFINE_F(ArbitrageStrategyFixture, BasicStrategySynchronous)
(benchmark::State &st) {
  using namespace dex;
  using namespace strategy;

  static auto strat = ArbitrageStrategy();

  auto poolA = Pool(Token::Symbol::LP_A, 1000, Token(Token::Symbol::DAI, 10000),
                    Token(Token::Symbol::ETH, 45));
  auto poolB = Pool(Token::Symbol::LP_B, 1000, Token(Token::Symbol::DAI, 11000),
                    Token(Token::Symbol::ETH, 30));
  for (auto _ : st) {
    strat.calculate(poolA, poolB);
  }
}

#define WITH_THREADS()                                                         \
  Threads(1)->Threads(2)->Threads(5)->Threads(10)->Threads(15)

BENCHMARK_REGISTER_F(ArbitrageStrategyFixture, BasicStrategyAsync)
    ->WITH_THREADS();
BENCHMARK_REGISTER_F(ArbitrageStrategyFixture, BasicStrategySynchronous)
    ->WITH_THREADS();

BENCHMARK_MAIN();