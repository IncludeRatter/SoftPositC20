#pragma once
#include "constants.hpp"
#include "itsy/bitsy.hpp"
#include "types.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>

// shift left that pads zeros on the right
void shiftLeft(BitSequence &bits, const unsigned int placesToShift) {
  for (size_t i = 0; i < placesToShift; i++) {
    bits.pop_front();
    bits.push_back(false);
  }
}

// shift right that pads zeros on the left 
void shiftRight(BitSequence &bits, const unsigned int placesToShift) {
  for (size_t i = 0; i < placesToShift; i++) {
    bits.pop_back();
    bits.push_front(false);
  }
}

// adds one to bit sequence
void increment(BitSequence &bits) {
  auto bitIterator = bits.end() - 1;
  while (bitIterator->flip() == 0) {
    --bitIterator;
  }
}

// flips all the bits
void flip(BitSequence &bits) {
  for (auto &&bit : bits) {
    bit.flip();
  }
}

// counts leading zeros or ones
unsigned int countLeadingOnesOrZeros(const BitSequence bits, bool onesOrZeros) {
  unsigned int counter = 0;
  for (auto &&bit : bits) {
    if (bit != onesOrZeros) {
      break;
    }
    counter++;
  }
  return counter;
}

// counts leading zeros or ones and shifts left bit sequence by counted number of zeros or ones.
// This uses shift left that pads with zeros
BitSequence lzocShift(BitSequence &bits, bool zeroOrOnes) {
  // count bits
  auto countedZerosOrOnes = countLeadingOnesOrZeros(bits, zeroOrOnes);
  // shift bits
  shiftLeft(bits, countedZerosOrOnes);
  return bits;
}

// or's all bits in sequence.
bool orReduce(const BitSequence posit) {
  if (posit.size() == 1)
    return *posit.begin();
  bool result = false;
  for (auto &&currentBit : posit) {
    result = result or currentBit;
  }
  return result;
}

// takes two's complement of a bit sequence.
BitSequence twosComplement(BitSequence bits) {
  // flip all the bits
  flip(bits);
  // add one
  increment(bits);
  return bits;
}


// construct unsigned int from bit sequence
// uses all bits in bit sequence
unsigned int convertBitSequenceToUInt(const BitSequence bits) {
  return std::accumulate(bits.begin(), bits.end(), 0ull,
                         [](auto acc, auto bit) { return (acc << 1) | bit; });
}

// construct unsigned int from bit sequence
// uses iterators for uint constructor
unsigned int convertBitSequenceToUInt(BitSequence::const_iterator begin,
                                      BitSequence::const_iterator end) {
  return std::accumulate(begin, end, 0ull,
                         [](auto acc, auto bit) { return (acc << 1) | bit; });
}

// constructs bit sequence from uint
BitSequence convertUIntToBitSequence(unsigned int bitPattern) {
  unsigned int mask = 0x1;
  BitSequence bits;
  for (int i = 0; i < 32; i++) {
    bool bit = bitPattern & mask;
    // add to vector
    bits.push_front(bit);
    mask = mask << 1;
  }
  return bits;
}