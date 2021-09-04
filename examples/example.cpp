#include "IO.hpp"
#include "Posit.hpp"
#include <iostream>

template <unsigned int N, unsigned int es>
void printDecodedBitFieldsOfPosit(Posit<N, es> p) {
  std::cout << "Sign bit:\n";
  printBits(p.getSignBit());
  std::cout << "Regime bits:\n";
  printBits(p.getRegimeBits());
  std::cout << "Exponent bits:\n";
  printBits(p.getExponentBits());
  std::cout << "Fraction bits:\n";
  printBits(p.getFractionBits());
}

int main() {
  // creating posit from bit sequence
  Posit<8, 0> p = BitSequence{0, 1, 0, 1, 0, 1, 0, 1};
  // decoding posit bit fields
  printDecodedBitFieldsOfPosit(p);
  // print value of posit
  std::cout << "Double value:\n";
  std::cout << p.getDouble() << std::endl;

  // Create posit from double or float
  double realX = 3.14;
  double realY = 6.14;
  Posit<32, 2> p1(realX);
  Posit<32, 2> p2(realY);

  // print value of posit
  std::cout << "Double value:\n";
  std::cout << p1.getDouble() << std::endl;

  // Comparing two posits:
  auto predicate = p1 < p2;
  std::cout << predicate << std::endl;
}