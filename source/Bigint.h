#pragma once

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

class BigInt {
public:
  BigInt() = default;
  BigInt(const std::vector<uint8_t> &, bool, const uint8_t &);
  BigInt(int64_t);
  BigInt(const BigInt &);
  BigInt(const BigInt &, const uint8_t &);
  BigInt(std::string, const uint8_t &);

  BigInt ToDecimal();
  BigInt Abs() const;
  BigInt Power(BigInt) const;
  int64_t ToDecimalInt() const;
  BigInt Convert(const uint8_t &) const;
  std::string ToString();

  uint8_t GetBase() const;
  int32_t GetLength() const;
  bool GetSign() const;

  void DeleteZeros();
  uint8_t CountLeadZeros();

private:
  std::vector<uint8_t> number;
  uint8_t base;
  bool sign;

public:
  uint8_t *operator[](int);

  friend BigInt operator-(BigInt);
  friend BigInt operator+(const BigInt &);
  friend BigInt operator+(const BigInt &, const BigInt &);
  friend BigInt operator-(const BigInt &, const BigInt &);
  friend BigInt operator*(const BigInt &, const BigInt &);
  friend BigInt operator*(const BigInt &, const int16_t &);
  friend BigInt operator/(const BigInt &, const BigInt &);
  friend BigInt operator/(const BigInt &, int64_t);
  friend std::ostream &operator<<(std::ostream &, const BigInt &);

  friend BigInt operator<<(const BigInt &, int);
  friend BigInt operator>>(const BigInt &, int);

  friend BigInt operator+=(BigInt &, BigInt);
  friend BigInt operator-=(BigInt &, BigInt);
  friend BigInt operator*=(BigInt &, BigInt);
  friend BigInt operator*=(BigInt &, int16_t);

public:
  friend bool operator<(const BigInt &, const BigInt &);
  friend bool operator>(const BigInt &, const BigInt &);
  friend bool operator==(const BigInt &, const BigInt &);
  friend bool operator<=(const BigInt &, const BigInt &);
  friend bool operator>=(const BigInt &, const BigInt &);
  friend bool operator!=(const BigInt &, const BigInt &);
};

BigInt gcd(BigInt, BigInt);

BigInt operator-(BigInt);
BigInt operator+(BigInt);
BigInt operator+(const BigInt &, const BigInt &);
BigInt operator-(const BigInt &, const BigInt &);
BigInt operator*(const BigInt &, const BigInt &);
BigInt operator*(const BigInt &, const int16_t &);
BigInt operator/(const BigInt &, const BigInt &);
BigInt operator%(const BigInt &, const BigInt &);
BigInt operator/(const BigInt &, int64_t);
BigInt operator%(const BigInt &, int64_t);
std::ostream &operator<<(std::ostream &, const BigInt &);

BigInt operator<<(const BigInt &, int);
BigInt operator>>(const BigInt &, int);

BigInt operator+=(BigInt &, BigInt);
BigInt operator-=(BigInt &, BigInt);
BigInt operator*=(BigInt &, BigInt);

bool operator<(const BigInt &, const BigInt &);
bool operator>(const BigInt &, const BigInt &);
bool operator==(const BigInt &, const BigInt &);
bool operator<=(const BigInt &, const BigInt &);
bool operator>=(const BigInt &, const BigInt &);
bool operator!=(const BigInt &, const BigInt &);
