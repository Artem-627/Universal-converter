#include "Bigint.h"
#include "BigFraction.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <endian.h>
#include <iostream>
#include <sys/types.h>
#include <utility>

BigInt::BigInt(const std::vector<uint8_t> &number, bool sign,
               const uint8_t &base) {
  this->number = number;
  this->sign = sign;
  this->base = base;
}

BigInt::BigInt(const BigInt &big_int) {
  this->number = big_int.number;
  this->sign = big_int.sign;
  this->base = big_int.base;
}

BigInt::BigInt(const BigInt &big_int, const uint8_t &base) {
  this->number = big_int.number;
  this->sign = big_int.sign;
  this->base = base;
}

BigInt::BigInt(int64_t number) {
  this->sign = (number < 0);
  this->number.clear();
  number = std::abs(number);

  while (number) {
    this->number.push_back(number % 10);
    number /= 10;
  }

  this->base = 10;
}

BigInt::BigInt(std::string number, const uint8_t &base) {
  this->base = base;
  if (number[0] == '-') {
    this->sign = 1;
    number.erase(0, 1);
  } else {
    this->sign = 0;
  }
  this->number.clear();
  for (auto str_digit = number.rbegin(); str_digit < number.rend();
       ++str_digit) {
    uint8_t digit;
    if (*str_digit >= '0' && *str_digit <= '9') {
      digit = *str_digit - '0';
    } else if (*str_digit >= 'A' && *str_digit <= 'Z') {
      digit = *str_digit - 'A' + 10;
    } else if (*str_digit >= 'a' && *str_digit <= 'z') {
      digit = *str_digit - 'a' + 10;
    } else if (*str_digit == '[') {
      ++str_digit;
    } else if (*str_digit == ']') {
      int res_digit = 0;
      ++str_digit;
      int curr_p = 1;
      while (*str_digit != '[') {
        res_digit += curr_p * (*str_digit - '0');
        ++str_digit;
        curr_p *= 10;
      }
      digit = static_cast<uint8_t>(res_digit);
    } else {
      throw std::runtime_error("Wrong digit (unknown symbol)");
    }
    if (digit >= base) {
      throw std::runtime_error("Wrong digit (digit >= base)");
    }
    this->number.push_back(digit);
  }
}

BigInt operator+=(BigInt &first, BigInt second) {
  BigInt res = first + second;

  first = res;
  return res;
}

BigInt operator-=(BigInt &first, BigInt second) {
  BigInt res = first - second;

  first = res;
  return res;
}

BigInt operator*=(BigInt &first, BigInt second) {
  BigInt res = first * second;

  first = res;
  return res;
}

BigInt operator*=(BigInt &first, int16_t second) {
  BigInt res = first * second;

  first = res;
  return res;
}

uint8_t BigInt::GetBase() const { return this->base; }
int32_t BigInt::GetLength() const { return this->number.size(); }
bool BigInt::GetSign() const { return this->sign; }

BigInt BigInt::Abs() const {
  BigInt res = *this;
  if (res.sign) {
    res = -res;
  }

  return res;
}

BigInt BigInt::Power(BigInt power_degree) const {
  BigInt res({1}, 0, this->base);
  BigInt power_base = *this;

  power_degree.DeleteZeros();
  while (power_degree > 0) {
    res *= power_base;

    power_degree -= BigInt(1);
    power_degree.DeleteZeros();
  }

  return res;
}

void BigInt::DeleteZeros() {
  while (this->number[this->number.size() - 1] == 0 &&
         this->number.size() != 1) {
    this->number.pop_back();
  }
}

uint8_t BigInt::CountLeadZeros() {
  uint8_t counter = 0;
  for (auto digit = this->number.rbegin(); digit < this->number.rend();
       ++digit) {
    if (*digit != 0) {
      break;
    }
    ++counter;
  }
  return counter;
}

uint8_t *BigInt::operator[](int index) { return &this->number[index]; }

BigInt operator-(BigInt big_int) {
  big_int.sign = !big_int.sign;
  return big_int;
}

BigInt operator+(const BigInt &big_int) { return big_int; }

bool operator<(const BigInt &first, const BigInt &second) {
  if ((first.sign == 1) && (second.sign == 0)) {
    return 1;
  } else if ((first.sign == 0) && (second.sign == 1)) {
    return 0;
  } else {
    if (first.GetLength() < second.GetLength()) {
      return (first.sign ? 0 : 1);
    } else if (first.GetLength() > second.GetLength()) {
      return (first.sign ? 1 : 0);
    } else {
      for (int i = first.GetLength() - 1; i >= 0; --i) {
        if (first.number[i] < second.number[i]) {
          return (first.sign ? 0 : 1);
        } else if (first.number[i] > second.number[i]) {
          return (first.sign ? 1 : 0);
        }
      }
    }
  }
  return 0;
}

bool operator>(const BigInt &first, const BigInt &second) {
  if ((first.sign == 0) && (second.sign == 1)) {
    return 1;
  } else if ((first.sign == 1) && (second.sign == 0)) {
    return 0;
  } else {
    if (first.GetLength() > second.GetLength()) {
      return (first.sign ? 0 : 1);
    } else if (first.GetLength() < second.GetLength()) {
      return (first.sign ? 1 : 0);
    } else {
      for (int i = first.GetLength() - 1; i >= 0; --i) {
        if (first.number[i] > second.number[i]) {
          return (first.sign ? 0 : 1);
        } else if (first.number[i] < second.number[i]) {
          return (first.sign ? 1 : 0);
        }
      }
    }
  }
  return 0;
}

bool operator==(const BigInt &first, const BigInt &second) {
  if ((first.GetLength() != second.GetLength()) || first.sign != second.sign) {
    return 0;
  } else {
    for (int i = first.GetLength() - 1; i >= 0; --i) {
      if (first.number[i] != second.number[i]) {
        return 0;
      }
    }
  }
  return 1;
}

bool operator==(const BigInt &first, const int &second) {
  return (first.ToDecimalInt() == second);
}

bool operator>=(const BigInt &first, const BigInt &second) {
  return first > second || first == second;
}

bool operator<=(const BigInt &first, const BigInt &second) {
  return first < second || first == second;
}

bool operator!=(const BigInt &first, const BigInt &second) {
  return !(first == second);
}

std::ostream &operator<<(std::ostream &os, const BigInt &big_int) {
  if (big_int.sign) {
    os << '-';
  }
  for (auto digit = big_int.number.rbegin(); digit < big_int.number.rend();
       ++digit) {
    if (*digit >= 0 && *digit <= 9) {
      os << static_cast<int>(*digit);
    } else if (*digit >= 10 && *digit <= 35) {
      os << (char)('A' + *digit - 10);
      // os << 'A' + *digit;
    } else {
      os << '[' << static_cast<int>(*digit) << ']';
    }
    // os << static_cast<int>(*digit);
    // os << '[' << static_cast<int>(*digit) << ']';
  }
  // os << "(" << static_cast<int>(big_int.GetBase()) << ")";
  return os;
}

BigInt operator+(const BigInt &first_, const BigInt &second_) {
  BigInt first = first_;
  BigInt second = second_;
  BigInt *res;
  std::vector<uint8_t> number;

  if ((first_.Abs() < second_.Abs()) &&
      (first_.GetSign() || second_.GetSign())) {
    std::swap(first, second);
  }

  if ((first.GetSign() && second.GetSign()) ||
      (first.GetSign() && !second.GetSign())) {
    first = -first;
    second = -second;
  }

  bool sign;
  if (first_.Abs() > second_.Abs()) {
    sign = first_.GetSign();
  } else {
    sign = second_.GetSign();
  }

  int32_t first_length = first.GetLength();
  int32_t second_length = second.GetLength();

  bool first_sign = first.GetSign();
  bool second_sign = second.GetSign();

  int16_t carry = 0;
  for (int i = 0; i < std::max(first_length, second_length); ++i) {
    int16_t a = (i < first_length ? *first[i] : 0);
    if (first_sign) {
      a = -a;
    }
    int16_t b = (i < second_length ? *second[i] : 0);
    if (second_sign) {
      b = -b;
    }

    if (a + b + carry >= first.GetBase()) {
      number.push_back(a + b + carry - first.GetBase());
      carry = 1;
    } else if (a + b + carry < 0) {
      if (i == std::max(first_length, second_length) - 1) {
        number.push_back(std::abs(a + b + carry));
      } else {
        number.push_back(a + b + carry + first.GetBase());
      }
      carry = -1;
    } else {
      number.push_back(a + b + carry);
      carry = 0;
    }
  }

  if (carry == 1) {
    number.push_back(1);
  }

  res = new BigInt(number, sign, first.GetBase());

  return *res;
}

BigInt operator-(const BigInt &first_, const BigInt &second_) {
  return first_ + (-second_);
}

BigInt operator<<(const BigInt &big_int, int range) {
  BigInt res = big_int;
  while (range--) {
    res.number.insert(res.number.begin(), 0);
  }
  return res;
}

BigInt operator>>(const BigInt &big_int, int range) {
  BigInt res = big_int;
  while (range--) {
    res.number.erase(res.number.begin());
  }
  return res;
}

BigInt operator*(const BigInt &first_, const BigInt &second_) {
  BigInt first = first_;
  BigInt second = second_;
  BigInt res({0}, 0, first.GetBase());

  bool sign = first.GetSign() ^ second.GetSign();
  first = first.Abs();
  second = second.Abs();

  for (int i = 0; i < second.GetLength(); ++i) {
    BigInt curr_mult({0}, 0, first.GetBase());
    for (int j = 0; j < *second[i]; ++j) {
      curr_mult += first;
    }
    curr_mult = curr_mult << i;
    res += curr_mult;
  }

  res.sign = sign;
  return res;
}

BigInt operator*(const BigInt &first_, const int16_t &second_) {
  BigInt first = first_;
  int16_t second = second_;
  BigInt res({0}, 0, first.GetBase());

  bool sign = first.GetSign() ^ (second_ < 0);
  first = first.Abs();
  second = std::abs(second);

  for (int i = 0; i < second; ++i) {
    res += first;
  }

  res.sign = sign;
  return res;
}

BigInt operator/(const BigInt &first_, int64_t denum) {
  if (denum == 0) {
    throw std::runtime_error("Division by zero");
  }
  BigInt first = first_;
  first.sign = (denum < 0) ^ first_.GetSign();
  denum = std::abs(denum);

  int carry = 0;
  for (auto digit = first.number.rbegin(); digit < first.number.rend();
       ++digit) {
    int64_t curr = *digit + carry * first.GetBase();
    *digit = uint8_t(curr / denum);
    carry = int(curr % denum);
  }

  return first;
}

BigInt operator%(const BigInt &first_, int64_t denum) {
  BigInt div = first_ / denum;
  div.DeleteZeros();

  BigInt res = first_ - (div * denum);
  res.DeleteZeros();
  return res;
}

BigInt operator/(const BigInt &first_, const BigInt &second_) {
  if (second_ == BigInt({0}, 0, 10) || second_ == BigInt({0}, 1, 10)) {
    throw std::runtime_error("Division by zero");
  }
  if (first_ == second_) {
    return BigInt({1}, 0, first_.GetBase());
  }
  if (second_.Abs() > first_.Abs()) {
    return BigInt({0}, 0, first_.GetBase());
  }
  BigInt first = first_;
  BigInt second = second_;

  first.DeleteZeros();
  second.DeleteZeros();

  if (first == 0) {
    return BigInt(0);
  }

  bool sign = first.GetSign() ^ second.GetSign();
  first = first.Abs();
  second = second.Abs();

  BigInt left({0}, 0, first.GetBase());
  BigInt right = first;

  while (right > left + 1) {
    left.DeleteZeros();
    right.DeleteZeros();
    BigInt middle = left + (right - left) / 2;

    middle.DeleteZeros();

    if (middle * second >= first) {
      right = middle;
    } else {
      left = middle;
    }
  }

  left.DeleteZeros();
  right.DeleteZeros();

  left.sign = sign;
  right.sign = sign;

  if (right * second_ == first_) {
    return right;
  } else if (left * second_ == first_) {
    return left;
  }

  return left;
}

BigInt operator%(const BigInt &first_, const BigInt &second_) {
  BigInt div = first_ / second_;
  div.DeleteZeros();

  BigInt mult = div * second_;
  mult.DeleteZeros();

  BigInt res = first_ - mult;
  res.DeleteZeros();
  return res;
}

int64_t BigInt::ToDecimalInt() const {
  int64_t result = 0;

  int64_t curr_p = 1;
  for (int i = 0; i < this->number.size(); ++i, curr_p *= this->base) {
    result += this->number[i] * curr_p;
  }

  return (this->sign ? -result : result);
}

std::string BigInt::ToString() {
  std::string res;

  for (auto digit = number.rbegin(); digit < number.rend(); ++digit) {
    if (*digit >= 0 && *digit <= 9) {
      res += *digit + '0';
    } else if (*digit >= 10 && *digit <= 35) {
      res += (char)('A' + *digit - 10);
    } else {
      res += '[';
      res += std::to_string(static_cast<int>(*digit));
      res += ']';
    }
  }

  return res;
}

BigInt BigInt::Convert(const uint8_t &base) const {
  BigInt result({0}, this->GetSign(), base);

  BigInt curr_p({1}, 0, base);
  for (int i = 0; i < this->number.size(); ++i, curr_p *= this->base) {
    result += curr_p * this->number[i];
  }

  result.sign = this->GetSign();
  return result;
}

BigInt gcd(BigInt a, BigInt b) {
  a.DeleteZeros();
  a = a.Abs();
  b.DeleteZeros();
  b = b.Abs();
  if (a < 1) {
    return b;
  }

  BigInt mod = b % a;
  mod.DeleteZeros();
  return gcd(mod, a);
}
