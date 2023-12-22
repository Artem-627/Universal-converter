#pragma once

#include "Bigint.h"
#include <iostream>
#include <vector>

class BigFraction {
private:
  BigInt num = 1;
  BigInt denum = 1;

public:
  BigFraction(const BigInt &, const BigInt &);
  BigFraction(const BigInt &);
  BigFraction() = default;

  void reduce();
  void DeleteZeros();

  uint8_t GetBase() const;
  BigInt GetInteger() const;
  BigInt GetFractional() const;
  BigInt GetNum() const;
  BigInt GetDenum() const;

  BigFraction Convert(const uint8_t &) const;

  friend std::ostream &operator<<(std::ostream &, const BigFraction &);
  friend BigFraction operator+(const BigFraction &, const BigFraction &);
};

std::ostream &operator<<(std::ostream &, const BigFraction &);

BigFraction operator+(const BigFraction &, const BigFraction &);
