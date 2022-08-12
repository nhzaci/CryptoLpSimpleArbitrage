#include <iostream>
#include <string>

#include "Token.hxx"

using namespace dex;

Token::Token(Token::Symbol _symbol, double _quantity)
    : symbol_{_symbol}, quantity_{_quantity} {};

void Token::mutateQuantity(double newQuantity) { quantity_ = newQuantity; }

Token Token::cloneWithNewQuantity(double newQuantity) {
  return Token(symbol_, newQuantity);
}

bool Token::isSameSymbol(const Token &other) {
  return other.symbol() == symbol();
}
