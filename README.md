# SoftPositCPP
This repository hosts C++ implementation of John Gustafson Posit Arithmetic. Underlying implementation uses itsy_bitsy library for bit manipulation.
# Requirements
- c++20 capable compiler, e.g.: gcc v. 10.2.0
- make
# Dependencies
This project depends on:
- bit manipulation library - itsy_bitsy https://github.com/ThePhD/itsy_bitsy
- reference Posit implementation - https://gitlab.com/cerlane/SoftPosit
- Catch2 - testing framework - https://github.com/catchorg/Catch2
- gcem - A C++ compile-time math library using generalized constant expressions - https://github.com/kthohr/gcem.git 
# Getting code
To get the code with neccessary dependencies use:
```
git clone --recursive https://github.com/IncludeRatter/SoftPositCPP.git
```
# Building
To build library you point your compiler to src directory and includes of itsy_bitsy library:
```
g++ your_main.cpp -o your_executable \
-Isrc/ \
-Ithird_party/itsy_bitsy/include/
```
# Testing
Before running tests you need to build reference library SoftPosit. To do that run:
```
make build_softposit
```
and then:
```
make build_tests
./test
```
# Usage
Library provides Posit<N,es> type, which can be constructed from bit sequence:
```
Posit<8,0> p = BitSequence{0,1,0,1,0,1,0,1};
std::cout<<p.getDoublePrecisionPositValue()<<std::endl;
```
To see other examples of usage check examples.cpp file.
