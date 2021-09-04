#pragma once
#include "itsy/bitsy.hpp"

// Type for dynamic size bit sequence
using BitSequence = bitsy::bit_sequence<std::vector<std::size_t>>;

// Type for fixed sized bit sequence
template<std::size_t width>
using FixedWidthBitSequence = bitsy::bit_sequence<std::vector<std::size_t>>;