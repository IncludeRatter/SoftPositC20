#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "Posit.hpp"
#include "catch_amalgamated.hpp"
#include "softposit_cpp.h"
#include "utils.h"
#include <cmath>
#include <limits>
#include <numeric>
#include <iomanip>
#include <bitset>
#include "gcem.hpp"

// Unit test for Posit type
TEST_CASE("Testing Posit<8,0> against mathematica") {
  Posit<8, 0> pos = BitSequence{0, 0, 0, 0, 0, 0, 1, 1};
  // regime bits are only zeros here
  REQUIRE(pos.getRegimeBits() == BitSequence{0, 0, 0, 0, 0});
  // regime value is -5 for this configuration
  REQUIRE(pos.getRegimeValue() == -5);
  // exponent bits are 0 since the type is Posit<8,0>
  REQUIRE(pos.getExponentBits() == BitSequence());
  // exponent value
  REQUIRE(pos.getExponentValue() == 0);
  // fraction bits, there is only {1} here
  REQUIRE(pos.getFractionBits() == BitSequence{1});
  // Fraction Value is 3/64 in Mathematica
  REQUIRE(pos.getFractionValue() == 1.5);
  // compute posit value
  REQUIRE(pos.getDouble() == 0.046875);
}

TEST_CASE("Testing Posit<16,1> against mathematica") {
  Posit<16, 1> pos =
      BitSequence{0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1};
  REQUIRE(pos.getRegimeBits() == BitSequence{1});
  REQUIRE(pos.getRegimeValue() == 0);
  REQUIRE(pos.getExponentBits() == BitSequence{1});
  REQUIRE(pos.getExponentValue() == 1);
  REQUIRE(pos.getFractionBits() ==
          BitSequence{1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1});
  double epsilon = 0.00001f;
  REQUIRE(std::fabs(pos.getFractionValue() - 1.85034) < epsilon);
  REQUIRE(std::fabs(pos.getDouble() - 3.70068) < epsilon);
}

TEST_CASE("Testing Posit<32,2> against mathematica") {
  Posit<32, 2> pos =
      BitSequence{0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1,
                  0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1};
  REQUIRE(pos.getRegimeBits() == BitSequence{1});
  REQUIRE(pos.getRegimeValue() == 0);
  REQUIRE(pos.getExponentBits() == BitSequence{1, 1});
  REQUIRE(pos.getExponentValue() == 3);
  REQUIRE(pos.getFractionBits() == BitSequence{1, 0, 1, 1, 0, 0, 1, 1, 0,
                                               1, 1, 0, 1, 0, 1, 1, 1, 0,
                                               1, 1, 0, 0, 1, 1, 0, 1, 1});
  // REQUIRE(pos.getFractionValue()==3.0);
  double epsilon = 0.00001f;
  REQUIRE(std::fabs(pos.getFractionValue() - 1.70086) < epsilon);
  REQUIRE(std::fabs(pos.getDouble() - 13.6069) < epsilon);
}

TEST_CASE("Test against softposit library") {
  posit32_t pA = convertDoubleToP32(3.14);
  posit32_t pB = convertDoubleToP32(6.14);
  posit32_t referencePosit = convertDoubleToP32(9.28);
  auto result = p32_add(pA, pB);
  REQUIRE(p32_eq(result, referencePosit));
  // Comparing against SoftPosit Library
  auto pAA = castP32(0xFFFFFFFF);
  auto pBB = castP32(0x0000FFFF);
  Posit<32, 2> pAAA = BitSequence{
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 // 0xFFFFFFFF
  };
  Posit<32, 2> pBBB = BitSequence{
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 // 0x0000FFFF
  };
  // Less Than
  REQUIRE((pAAA < pBBB) == p32_lt(pAA, pBB));
  // Less or Equal
  REQUIRE((pAAA <= pBBB) == p32_le(pAA, pBB));
  // Equal
  REQUIRE((pAAA == pBBB) == p32_eq(pAA, pBB));
  // Addition - TO-DO
  // Subtraction - TO-DO
  // Multiplication - TO-DO
  // Division - TO-DO
}

TEST_CASE("Constructors from real"){
  double real = 3.1400000000000000000;
  Posit<32,2> p(real);
  auto libraryReferenceValue = convertP32ToDouble(convertDoubleToP32(real));
  double epsilon = 0.00001f;
  REQUIRE(std::fabs(p.getDouble() - libraryReferenceValue) < epsilon);
}