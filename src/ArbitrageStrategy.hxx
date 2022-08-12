#pragma once

#include <future>
#include <utility>

#include "Pool.hxx"

namespace strategy {

class ArbitrageStrategy {
public:
  /**
   * @brief Input two different pools to calculate if there's an arbitrage
   * opportunity
   * @return std::pair<eth required, profit>
   */
  std::pair<double, double> calculate(const dex::Pool &a,
                                      const dex::Pool &b) const;

  std::future<std::pair<double, double>>
  calculateAsync(const dex::Pool &a, const dex::Pool &b) const;
};

} // namespace strategy