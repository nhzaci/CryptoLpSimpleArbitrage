#include <iostream>

#include "ArbitrageStrategy.hxx"
#include "Pool.hxx"

int main() {
  using namespace std;

  double ethA, daiA, ethB, daiB;
  cout << "Enter the ETH quantity for Pool A (X_1):" << endl;
  cin >> ethA;
  cout << "Enter the DAI quantity for Pool A (Y_1):" << endl;
  cin >> daiA;
  auto poolA = dex::Pool(dex::Token::Symbol::LP_A,
                         dex::Token(dex::Token::Symbol::DAI, daiA),
                         dex::Token(dex::Token::Symbol::ETH, ethA));

  cout << "Enter the ETH quantity for Pool B (X_2):" << endl;
  cin >> ethB;
  cout << "Enter the DAI quantity for Pool B (Y_2):" << endl;
  cin >> daiB;
  auto poolB = dex::Pool(dex::Token::Symbol::LP_B,
                         dex::Token(dex::Token::Symbol::DAI, daiB),
                         dex::Token(dex::Token::Symbol::ETH, ethB));

  cout << "Calculating async call" << endl;
  auto strat = strategy::ArbitrageStrategy();
  auto future = strat.calculateAsync(poolA, poolB);
  auto [ethRequired, profit] = future.get();

  cout << "You will need: " << ethRequired
       << " ETH; and you will profit: " << profit << endl;
}