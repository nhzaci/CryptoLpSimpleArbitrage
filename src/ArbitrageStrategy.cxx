#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>

#include "ArbitrageStrategy.hxx"
#include "Pool.hxx"

std::pair<double, double>
strategy::ArbitrageStrategy::calculate(const dex::Pool &a,
                                       const dex::Pool &b) const {
  const double ethDaiRatioA = a.y().symbol() == dex::Token::Symbol::ETH
                                  ? a.y().quantity() / a.x().quantity()
                                  : a.x().quantity() / a.y().quantity();
  const double ethDaiRatioB = b.y().symbol() == dex::Token::Symbol::ETH
                                  ? b.y().quantity() / b.x().quantity()
                                  : b.x().quantity() / b.y().quantity();

  const double spread = fabs(ethDaiRatioA - ethDaiRatioB);
  constexpr const double lotSize = 0.01;

  // if no money to be made, don't execute
  if (std::fabs(spread) <= std::numeric_limits<double>::epsilon())
    return std::make_pair(0, 0);

  // find cross point
  double ethRequired =
      spread / ((1 / a.x().quantity()) + (1 / b.x().quantity()));
  double profit = ethRequired * spread *
                  ((1 - a.SWAP_FEE_PERCENTAGE) + (1 - b.SWAP_FEE_PERCENTAGE)) /
                  2;

  return std::make_pair(ethRequired, profit);
}

std::future<std::pair<double, double>>
strategy::ArbitrageStrategy::calculateAsync(const dex::Pool &a,
                                            const dex::Pool &b) const {
  return std::async(std::launch::async, [&] { return calculate(a, b); });
}