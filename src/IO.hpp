#pragma once
#include "types.hpp"
#include <iostream>

// print fixed size bit sequence
template <unsigned int size>
void printBits(const FixedWidthBitSequence<size> bits) {
  if (bits.size() == 0)
    return;
  for (auto &&i : bits) {
    std::cout << i;
  }
  std::cout << std::endl;
}

// print dynamic size bit sequence
void printBits(const BitSequence bits) {
  if (bits.size() == 0)
    return;
  for (auto &&i : bits) {
    std::cout << i;
  }
  std::cout << std::endl;
}