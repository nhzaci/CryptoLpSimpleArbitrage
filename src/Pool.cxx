#include <limits>
#include <tuple>
#include <utility>

#include "Pool.hxx"
#include "Token.hxx"

using namespace dex;

Token Pool::add(Token token1, Token token2) {
  return Token(Token::Symbol::LP_A, 20);
}

std::pair<Token, Token> Pool::withdraw(Token &otherLpToken) {
  bool isLpToken = otherLpToken.isSameSymbol(lpToken_);
  if (not isLpToken) {
    throw PoolException(PoolException::Type::InvalidTokenSymbol,
                        "Invalid namespace symbol not same as lp token symbol");
  }
  if (lpToken_.quantity() == 0) {
    throw PoolException(PoolException::Type::NoLpTokensMinted,
                        "Lp token quantity is 0 in pool");
  }

  double lpTokenProportion = otherLpToken.quantity() / lpToken_.quantity();

  auto reduceSupplyAndReturnToken = [&](const double lpTokenProportion,
                                        Token &tokenToReturn) -> Token {
    double tokenToReturnQuantity = lpTokenProportion * tokenToReturn.quantity();
    tokenToReturn.mutateQuantity(tokenToReturn.quantity() -
                                 tokenToReturnQuantity);

    calculateK();

    return tokenToReturn.cloneWithNewQuantity(tokenToReturnQuantity);
  };

  lpToken_.mutateQuantity(lpToken_.quantity() - otherLpToken.quantity());

  return std::make_pair(reduceSupplyAndReturnToken(lpTokenProportion, x_),
                        reduceSupplyAndReturnToken(lpTokenProportion, y_));
}

Token Pool::swap(Token &token) {
  bool isTokenX = token.isSameSymbol(x_);
  bool isTokenY = token.isSameSymbol(y_);

  if (not isTokenX and not isTokenY) {
    throw PoolException(
        PoolException::Type::InvalidTokenSymbol,
        "Invalid token symbol does not belong to either tokens in pool.");
  }

  auto swapToken = [&](const Token &userTokensToSwap, Token &poolToSwap,
                       Token &poolToReceive) -> Token {
    double currentRatio = poolToReceive.quantity() / poolToSwap.quantity();
    double amountUserReceives = currentRatio * userTokensToSwap.quantity();
    double fee = userTokensToSwap.quantity() * SWAP_FEE_PERCENTAGE;

    if (poolToReceive.quantity() - amountUserReceives < 0) {
      throw PoolException(
          PoolException::Type::InsufficientLiquidity,
          "Insufficient amount of tokens to complete your swap.");
    }

    poolToSwap.mutateQuantity(poolToSwap.quantity() + fee +
                              userTokensToSwap.quantity());

    poolToReceive.mutateQuantity(poolToReceive.quantity() - amountUserReceives);

    calculateK();

    return poolToReceive.cloneWithNewQuantity(amountUserReceives);
  };

  return isTokenX ? swapToken(token, x_, y_) : swapToken(token, y_, x_);
}

std::tuple<Token, Token, double> Pool::status() const {
  return std::make_tuple(x_, y_, k_);
}