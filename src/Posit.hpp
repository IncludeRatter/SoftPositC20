#pragma once
#include "types.hpp"
#include "bit_manipulation.hpp"
#include "posit_internals.hpp"
#include <cmath>
#include <numeric>
#include "gcem.hpp"

template <const unsigned int positSize, const unsigned int exponentSize>
class Posit {
private:
  // fields
  FixedWidthBitSequence<positSize> bits;
  // change with compile time computations
  constexpr static unsigned long base = 2;

  constexpr static unsigned int nbits = positSize;
  constexpr static unsigned int useed = gcem::pow(base,gcem::pow(base,exponentSize));
  constexpr static long long npat = gcem::pow(base,positSize);
public:
  // getters setters
  FixedWidthBitSequence<positSize> getBitSequence() const { return this->bits; }
  void setBitSequence(FixedWidthBitSequence<positSize> bits) {
    this->bits = bits;
  }
  const unsigned int getExponentSize() { return exponentSize; }
  const unsigned int getPositSize() { return positSize; }
  // decoding
  bool getSignBit() { return signBit(this->bits); }
  BitSequence getExponentBits() {
    return exponentBitsNaive(this->bits, getRegimeBits().size(),
                             this->getExponentSize());
  }
  const int getExponentValue() { return exponentValue(getExponentBits()); }
  BitSequence getRegimeBits() { return regimeBitsNaive(this->bits); }
  const int getRegimeValue() { return regimeValue(getRegimeBits()); }
  BitSequence getFractionBits() {
    return fractionBitsNaive(this->bits, getRegimeBits().size(),
                             this->getExponentSize());
  }
  double getFractionValue() { return fractionValue(getFractionBits()); }
  // get values
  double getDouble(){
    auto sign = getSignBit();
    auto regime = getRegimeBits();
    auto exponent = getExponentBits();
    auto fraction = getFractionBits();
    return positToDouble(sign,regime,exponent,fraction,this->useed);
  }
  // Operators
  bool operator<(const Posit<positSize, exponentSize> other) const {
    auto x = (signed int)convertBitSequenceToUInt(this->getBitSequence());
    auto y = (signed int)convertBitSequenceToUInt(other.getBitSequence());
    return x < y;
  }
  bool operator<=(const Posit other) const {
    auto x = (signed int)convertBitSequenceToUInt(this->getBitSequence());
    auto y = (signed int)convertBitSequenceToUInt(other.getBitSequence());
    return x <= y;
  }
  bool operator>(const Posit other) const {
    auto x = (signed int)convertBitSequenceToUInt(this->getBitSequence());
    auto y = (signed int)convertBitSequenceToUInt(other.getBitSequence());
    return x > y;
  }
  bool operator>=(const Posit other) const {
    auto x = (signed int)convertBitSequenceToUInt(this->getBitSequence());
    auto y = (signed int)convertBitSequenceToUInt(other.getBitSequence());
    return x >= y;
  }
  bool operator==(const Posit other) {
    return (this->getBitSequence() == other.getBitSequence());
  }
  /*
  Posit operator+(const Posit other){
    return Posit(BitSequence{0,1});
  }
  Posit operator-(const Posit other){
          return Posit(BitSequence{0,1});
  }
  Posit operator*(const Posit other){
          return Posit(BitSequence{0,1});
  }
  Posit operator/(const Posit other){
          return Posit(BitSequence{0,1});
  }
  */
  // constructors
  Posit() { }
  // From explicit bit sequence
  Posit(FixedWidthBitSequence<positSize> bits) {
    this->bits = bits;
  }
  // constructors from double
  Posit(double real) {
    auto positBitPattern = x2p(real,exponentSize,this->nbits,this->useed,this->npat);
    // convert uint to BitSequence
    auto bits = convertUIntToBitSequence(positBitPattern);
    this->setBitSequence(bits);
  }
  // does it work for float?
  Posit(float real) {
    auto positBitPattern = x2p(real,exponentSize,this->nbits,this->useed,this->npat);
    // convert uint to BitSequence
    auto bits = convertUIntToBitSequence(positBitPattern);
    this->setBitSequence(bits);
  }
  ~Posit() {}
};
