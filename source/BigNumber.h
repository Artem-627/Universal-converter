#pragma once

#include "BigFraction.h"
#include "Bigint.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

class BigNumber {
private:
  BigFraction integer;
  BigFraction fractional;
  BigFraction period;

  std::vector<int> PrefixFunction(std::string s) const;

  bool has_fractional = 1;

public:
  BigNumber(BigInt, BigInt, BigInt);
  BigNumber(BigFraction, BigFraction, BigFraction);
  BigNumber(std::string, const uint8_t &);

  BigInt GetInteger() const;
  BigInt GetFractional() const;
  std::string ToString();

  BigNumber Convert(const uint8_t &) const;
  std::string ToFloatNumber();

  friend std::ostream &operator<<(std::ostream &, const BigNumber &);
};
