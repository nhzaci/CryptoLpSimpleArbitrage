#pragma once

#include <cstdint>
#include <iostream>
#include <string>

namespace dex {

class Token {
public:
  enum Symbol { ETH, DAI, LP_A, LP_B };

  Token(Token::Symbol, double);
  Token() = delete;
  ~Token() = default;

  Token(const Token &) = default;
  Token &operator=(const Token &) = default;

  Token(Token &&) = default;
  Token &operator=(Token &&) = default;

  double quantity() const { return quantity_; }
  const Token::Symbol &symbol() const { return symbol_; }

  void mutateQuantity(double);
  Token cloneWithNewQuantity(double);
  bool isSameSymbol(const Token &);

private:
  Token::Symbol symbol_;
  double quantity_;
};

inline std::ostream &operator<<(std::ostream &os, const Token::Symbol &symbol) {
  switch (symbol) {
  case Token::Symbol::ETH:
    return os << "ETH";
  case Token::Symbol::DAI:
    return os << "DAI";
  case Token::Symbol::LP_A:
    return os << "LP: A";
  case Token::Symbol::LP_B:
    return os << "LP: B";
  default:
    return os << "Unknown token symbol";
  }
  return os;
}

inline std::ostream &operator<<(std::ostream &os, const Token &token) {
  return os << "Token(symbol=" << token.symbol()
            << ", quantity=" << token.quantity() << ")";
}

}; // namespace dex
