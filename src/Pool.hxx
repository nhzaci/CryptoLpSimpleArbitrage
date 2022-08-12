#pragma once

#include <exception>
#include <iostream>
#include <memory>
#include <tuple>
#include <utility>

#include "Token.hxx"

namespace dex {

class Pool {
protected:
  Token x_;
  Token y_;
  double k_;

  Token lpToken_;

public:
  inline static constexpr double SWAP_FEE_PERCENTAGE = 0.003;

  Pool(Token::Symbol lpTokenSymbol, Token &&_t1, Token &&_t2)
      : lpToken_{Token(lpTokenSymbol, 0)}, x_{std::move(_t1)}, y_{std::move(
                                                                   _t2)} {
    calculateK();
  };
  Pool(Token::Symbol lpTokenSymbol, double lpTokenQuantity, Token &&_t1,
       Token &&_t2)
      : lpToken_{Token(lpTokenSymbol, lpTokenQuantity)}, x_{std::move(_t1)},
        y_{std::move(_t2)} {
    calculateK();
  };

  Pool() = delete;
  ~Pool() = default;
  Pool(const Pool &) = delete;
  Pool &operator=(const Pool &) = delete;
  Pool(Pool &&) = default;
  Pool &operator=(Pool &&) = default;

  const double k() const { return k_; }
  const Token &x() const { return x_; }
  const Token &y() const { return y_; }
  const Token &lpToken() const { return lpToken_; }

  /** Should only be recalculated on init and on swap */
  void calculateK() { k_ = x_.quantity() * y_.quantity(); }

  /**
   * @brief Takes in two tokens pool is currently using and returns LP tokens
   * @return LP Token
   */
  Token add(Token, Token);

  /**
   * @brief Takes in LP tokens and returns pair of initial tokens
   * @return std::pair<Token, Token> token1 and token2 pool holds onto,
   * proportional to LP tokens received
   */
  std::pair<Token, Token> withdraw(Token &);

  /**
   * @brief Swaps one token for another, taking in swap fee
   * @return Token swapped
   */
  Token swap(Token &);

  /**
   * @return std::tuple<Token, Token, double> status of two tokens with
   * quantities and a double indicating current k
   */
  std::tuple<Token, Token, double> status() const;
};

inline std::ostream &operator<<(std::ostream &os, const Pool &pool) {
  return os << "Pool(x=" << pool.x() << ", y=" << pool.y() << ", k=" << pool.k()
            << ", lp tokens: " << pool.lpToken() << ");";
}

class PoolException : public std::exception {
public:
  enum Type { InvalidTokenSymbol, NoLpTokensMinted, InsufficientLiquidity };

  PoolException(PoolException::Type _type, std::string _msg)
      : message{_msg}, type{_type} {};

  const char *what() const noexcept override { return message.c_str(); }

  friend std::ostream &operator<<(std::ostream &os,
                                  const PoolException::Type &type);

private:
  PoolException::Type type;
  std::string message;
};

inline std::ostream &operator<<(std::ostream &os,
                                const PoolException::Type &type) {
  using namespace dex;

  switch (type) {
  case PoolException::Type::InvalidTokenSymbol:
    return os << "Invalid token is not present in pool.";
  case PoolException::Type::NoLpTokensMinted:
    return os << "There were no LP tokens minted in pool.";
  case PoolException::Type::InsufficientLiquidity:
    return os << "There is not enough liquidity to perform your swap.";
  }
  return os << "Unknown pool exception type";
}

}; // namespace dex
