#pragma once
#include "types.hpp"
#include "constants.hpp"
#include "bit_manipulation.hpp"

// sign bit extraction
bool signBit(const BitSequence posit) { return *posit.begin(); }

// naive extraction exponent bits
BitSequence exponentBitsNaive(const BitSequence posit,
                              const unsigned int regimeBitsLength,
                              const unsigned int exponentSize) {
  const auto startBit = regimeBitsLength + exponentExtractionOffset;
  if (startBit > posit.size()) {
    return BitSequence();
  } else {
    const auto endOrShorter =
        std::min((int)(startBit + exponentSize - 1), (int)posit.size());
    return BitSequence(posit.begin() + startBit - 1,
                       posit.begin() + endOrShorter);
  }
}

// value of exponent of posit
int exponentValue(const BitSequence exponentBits) {
  if (exponentBits.size() == 0) {
    return 0;
  } else if (exponentBits.size() == 1) {
    return exponentBits.front();
  }
  return convertBitSequenceToUInt(exponentBits.begin(), exponentBits.end());
}

// naive decoding regime bits 
BitSequence regimeBitsNaive(const BitSequence posit) {
  // record whats the first bit after sign bit
  bool firstBit = *(posit.begin() + 1);
  auto bitIterator = (posit.begin() + 1);
  for (; bitIterator != posit.end(); ++bitIterator) {
    if (bitIterator->value() != firstBit) {
      break;
    }
  }
  return BitSequence((posit.begin() + 1), bitIterator);
}

// value of regime bits of posit
int regimeValue(const BitSequence regimeBits) {
  if (signBit(regimeBits) == true) {
    return regimeBits.size() - 1;
  }
  return -1 * regimeBits.size();
}

// naive decoding of fraction bits
BitSequence fractionBitsNaive(const BitSequence posit,
                              const unsigned int regimeBitsLength,
                              const unsigned int exponentSize) {
  auto startBit = regimeBitsLength + fractionExtractionOffset + exponentSize;
  if (startBit > posit.size())
    return BitSequence();
  return BitSequence(posit.begin() + startBit, posit.end());
}

// value of fraction bit field of posit
double fractionValue(const BitSequence fractionBits) {
  if (fractionBits.size() == 0) {
    return 1.0;
  }
  double maxFraction = std::pow(2, fractionBits.size());
  auto fractionValue =
      convertBitSequenceToUInt(fractionBits.begin(), fractionBits.end());
  auto result = 1.0 + ((double)fractionValue / maxFraction);
  return result;
}

// conversion of posit to real value (approximated with double precision number)
double positToDouble(const bool signBit, const BitSequence regimeBits,
                     const BitSequence exponentBits,
                     const BitSequence fractionBits, const unsigned int useed) {
  auto regValue = regimeValue(regimeBits);
  auto expValue = exponentValue(exponentBits);
  auto fracValue = fractionValue(fractionBits);
  auto sign = (int)signBit;
  auto useedK = std::pow(useed, regValue);
  auto signOfResult = std::pow(-1, sign);
  auto twoE = std::pow(2, expValue);
  return signOfResult * useedK * twoE * fracValue;
}

unsigned int x2p(double x, const unsigned int es, const unsigned int nbits,
                 const unsigned int useed, const long npat) {
  // Take absolute value of x for easier computation
  double y = std::abs(x);
  unsigned int i, p; // i - iterator over bits of double, p - resulting posit
  double e = std::pow(2, es - 1); // exponent value
  // First take care of the two exception values
  if (y == 0) {
    return 0;
  } else if (y == std::numeric_limits<double>::infinity()) {
    int n = 1;
    return n << (nbits - 1);
  } else {
    if (y >= 1) // Northeast quadrant
    {
      p = 1;
      i = 2; // Shift in 1s from the right and scale down
      while (y >= useed && i < nbits) {
        p = 2 * p + 1;
        y = y / useed;
        i = i + 1;
      }
      p = 2 * p;
      i++;
    } else // Southeast quadrant
    {
      p = 0;
      i = 1;
      while (y < 1 && i <= nbits) {
        y = y * useed;
        i = i + 1;
      }
      // Extract exponent bits
      if (i >= nbits) {
        p = 2;
        i = nbits + 1;
      } else {
        p = 1;
        i++;
      }
    }
    while (e > 0.5 && i <= nbits) {
      p = 2 * p;
      if (y >= std::pow(2, e)) {
        y /= std::pow(2, e);
        p++;
      }
      e /= 2;
      i++;
    }
    y--; // Fraction bits; subtract the hidden bit
    while (y > 0 && i <= nbits) {
      y = 2 * y;
      p = 2 * p + std::floor(y);
      y -= std::floor(y);
      i++;
    }
    p *= std::pow(2, nbits + 1 - i);
    i++;
    // Round to nearest; tie to even
    int mask = 1;
    i = p & mask;
    p = std::floor(p / 2);
    if (i == 0) {
      return p;                  // closer to lower value
    } else if (y == 1 || y == 0) // ties foes to nearest even
    {
      int mask = 1;
      p = p & mask;
      return p;
    } else // closer to upper value
    {
      // return value as twos complement
      if (x < 0) {
        p = npat - p;
      }
      return p % npat;
    }
  }
}

double p2x(unsigned int bitPattern, const unsigned int es,
           const unsigned int useed, const long npat) {
  unsigned int mask = 0x1;
  BitSequence bits;
  for (int i = 0; i < 32; i++) {
    bool bit = bitPattern & mask;
    // add to vector
    bits.push_front(bit);
    mask = mask << 1;
  }
  auto sBit = signBit(bits);
  auto regBits = regimeBitsNaive(bits);
  auto expBits = exponentBitsNaive(bits, regBits.size(), 2);
  unsigned int howManyZeros = es - expBits.size();
  for (unsigned int i = 0; i < howManyZeros; i++) {
    expBits.push_back(0);
  }
  auto fracBits = fractionBitsNaive(bits, regBits.size(), 2);
  auto sign = std::pow(-1, (int)sBit);
  auto regime = regimeValue(regBits);
  auto exponent = exponentValue(expBits);
  auto fraction = fractionValue(fracBits);

  if (bitPattern == 0) {
    return 0;
  } else if (bitPattern == npat / 2) {
    return std::numeric_limits<double>::infinity();
  } else {
    return sign * std::pow(useed, regime) * std::pow(2, exponent) * fraction;
  }
}