#pragma once

#include <gtest/gtest.h>

#include "Pool.hxx"
#include "Token.hxx"

using namespace dex;

class PoolTest : public ::testing::Test {
protected:
  void SetUp() override {}

  static constexpr double ethTokenQuantityA = 20;
  static constexpr double daiTokenQuantityA = 10000;
  static constexpr double lpTokenQuantityA = 1000;

  static constexpr double ethTokenQuantityB = 30;
  static constexpr double daiTokenQuantityB = 11000;

  dex::Pool poolA{Token::Symbol::LP_A, 1000,
                  Token(Token::Symbol::DAI, daiTokenQuantityA),
                  Token(Token::Symbol::ETH, ethTokenQuantityA)};
  dex::Pool poolB{Token::Symbol::LP_B,
                  Token(Token::Symbol::DAI, daiTokenQuantityB),
                  Token(Token::Symbol::ETH, ethTokenQuantityB)};
};

TEST_F(PoolTest, InvariantKIsCorrect) {
  EXPECT_DOUBLE_EQ(poolA.k(), ethTokenQuantityA * daiTokenQuantityA);
  EXPECT_DOUBLE_EQ(poolB.k(), ethTokenQuantityB * daiTokenQuantityB);
}

TEST_F(PoolTest, SwapCorrectTokensAndUpdatePoolK) {
  Token ethToken = Token(Token::Symbol::ETH, 5.2);
  double currentRatio = daiTokenQuantityA / ethTokenQuantityA;
  double amountToReceive = currentRatio * ethToken.quantity();

  double initialK = poolA.k();

  Token received = poolA.swap(ethToken);

  EXPECT_DOUBLE_EQ(received.quantity(), amountToReceive);
  EXPECT_EQ(received.symbol(), Token::Symbol::DAI);
}

TEST_F(PoolTest, Swap100DaiToEth) {
  Token daiToken = Token(Token::Symbol::DAI, 100);
  double currentRatio = ethTokenQuantityA / daiTokenQuantityA;
  double amountToReceive = currentRatio * daiToken.quantity();

  Token received = poolA.swap(daiToken);

  EXPECT_DOUBLE_EQ(received.quantity(), amountToReceive);
  EXPECT_EQ(received.symbol(), Token::Symbol::ETH);
}

TEST_F(PoolTest, ThrowPoolExceptionOnInvalidTokenForSwap) {
  Token lpToken = Token(Token::Symbol::LP_B, 10);
  ASSERT_THROW(poolA.swap(lpToken), PoolException);
}

TEST_F(PoolTest, ThrowPoolExceptionOnInvalidLpTokenForWithdrawal) {
  Token lpToken = Token(Token::Symbol::LP_B, 10);
  ASSERT_THROW(poolA.withdraw(lpToken), PoolException);
}

TEST_F(PoolTest, ThrowPoolExceptionIfNoLpTokensWereMinted) {
  Token lpToken = Token(Token::Symbol::LP_B, 10);
  ASSERT_THROW(poolB.withdraw(lpToken), PoolException);
}

TEST_F(PoolTest, ThrowPoolExceptionWhenQuantityTooLarge) {
  Token ethToken = Token(Token::Symbol::ETH, poolA.y().quantity() + 1);
  ASSERT_THROW(poolA.swap(ethToken), PoolException);
}

TEST_F(PoolTest, WithdrawMaintainsSameReserveRatio) {
  double lpTokenWithdrawQuantity = 10;
  Token lpToken = Token(Token::Symbol::LP_A, lpTokenWithdrawQuantity);

  auto [daiToken, ethToken] = poolA.withdraw(lpToken);

  double ethTokenExpectedQuantity =
      ethTokenQuantityA * lpTokenWithdrawQuantity / lpTokenQuantityA;
  double daiTokenExpectedQuantity =
      daiTokenQuantityA * lpTokenWithdrawQuantity / lpTokenQuantityA;

  EXPECT_DOUBLE_EQ(ethToken.quantity(), ethTokenExpectedQuantity);
  EXPECT_DOUBLE_EQ(daiToken.quantity(), daiTokenExpectedQuantity);

  EXPECT_DOUBLE_EQ(poolA.y().quantity(),
                   ethTokenQuantityA - ethToken.quantity());
  EXPECT_DOUBLE_EQ(poolA.x().quantity(),
                   daiTokenQuantityA - daiToken.quantity());
}
