#include "BigNumber.h"
#include "BigFraction.h"
#include "Bigint.h"
#include <algorithm>
#include <cinttypes>
#include <string>

BigNumber::BigNumber(BigInt integer, BigInt fractional, BigInt period = 0) {
  this->integer = BigFraction(integer, BigInt({1}, 0, integer.GetBase()));

  BigInt fractional_length = fractional.GetLength();
  BigInt fractional_base = fractional.GetBase();
  fractional_length = fractional_length.Convert(fractional.GetBase());
  fractional_base = fractional_base.Convert(fractional.GetBase());

  fractional.DeleteZeros();

  BigInt fractional_denum = fractional_base.Power(fractional_length);
  this->fractional = BigFraction(fractional, fractional_denum);

  // this->fractional.SetBase(1)

  BigInt period_length = period.GetLength();
  BigInt period_base = period.GetBase();
  period_length = period_length.Convert(period.GetBase());
  period_base = period_base.Convert(period.GetBase());

  // BigInt period_denum = period_base.Power(period_length + 1);
  BigInt period_denum = period_base.Power(period_length);
  period_denum -= 1;
  BigInt bias = period_base.Power(fractional_length - 1);
  period_denum = period_denum * bias;

  this->period = BigFraction(period, period_denum);
  this->period.DeleteZeros();

  this->integer.reduce();
  this->fractional.reduce();
  this->period.reduce();
}

BigNumber::BigNumber(BigFraction integer, BigFraction fractional,
                     BigFraction period) {
  this->integer = integer;
  this->fractional = fractional;
  this->period = period;

  this->integer.reduce();
  this->fractional.reduce();
  this->period.reduce();
}

BigNumber::BigNumber(std::string number_, const uint8_t &base) {
  std::string integer;
  std::string fractional;
  std::string period;

  auto curr_symbol = number_.begin();

  while (*curr_symbol != '.' && curr_symbol < number_.end()) {
    integer += *curr_symbol;
    ++curr_symbol;
  }
  if (*curr_symbol == '.') {
    ++curr_symbol;
  }

  while (*curr_symbol != '(' && curr_symbol < number_.end()) {
    fractional += *curr_symbol;
    ++curr_symbol;
  }
  if (*curr_symbol == '(') {
    ++curr_symbol;
  }

  while (*curr_symbol != ')' && curr_symbol < number_.end()) {
    period += *curr_symbol;
    ++curr_symbol;
  }

  if (fractional.length() == 0) {
    this->has_fractional = 0;
  }

  BigInt integer_num(integer, base);
  BigInt integer_denum("1", base);

  this->integer = BigFraction(integer_num, integer_denum);

  int fractional_length_int = 0;
  bool fractional_mode = 1;
  for (auto symbol = fractional.begin(); symbol < fractional.end(); ++symbol) {
    // if (*symbol != '[' && *symbol != ']') {
    // ++fractional_length_int;
    // }
    if (fractional_mode) {
      ++fractional_length_int;
    }
    if (*symbol == '[') {
      fractional_mode = 0;
    } else if (*symbol == ']') {
      fractional_mode = 1;
    }
  }
  BigInt fractional_length = BigInt(std::to_string(fractional_length_int), 100);
  fractional_length = fractional_length.Convert(base);
  BigInt fractional_base = BigInt(base);
  fractional_base = fractional_base.Convert(base);
  fractional_length = fractional_length.Convert(base);
  fractional_base = fractional_base.Convert(base);

  BigInt fractional_denum = fractional_base.Power(fractional_length);
  BigInt fractional_bigint;
  if (fractional.length() != 0) {
    fractional_bigint = BigInt(fractional, base);
  } else {
    fractional_bigint = BigInt("0", base);
  }

  this->fractional = BigFraction(fractional_bigint, fractional_denum);
  int period_length_int = 0;
  bool period_mode = 1;
  for (auto symbol = period.begin(); symbol < period.end(); ++symbol) {
    if (*symbol != '[' && *symbol != ']') {
      if (period_mode) {
        ++period_length_int;
      }
      if (*symbol == '[') {
        period_mode = 0;
      } else if (*symbol == ']') {
        period_mode = 1;
      }
    }
  }
  BigInt period_length = BigInt(std::to_string(period_length_int), 10);
  period_length = period_length.Convert(base);
  BigInt period_base = BigInt(base);
  period_base = period_base.Convert(base);
  period_length = period_length.Convert(base);
  period_base = period_base.Convert(base);

  BigInt period_denum;
  period_denum = period_base.Power(period_length);

  period_denum -= 1;
  BigInt bias;
  if (this->has_fractional) {
    bias = period_base.Power(fractional_length);
  } else {
    bias = period_base.Power(fractional_length - 1);
  }

  period_denum = period_denum * bias;

  BigInt period_num;
  if (period_length != 0) {
    period_num = BigInt(period, base);
  } else {
    period_num = BigInt("0", base);
  }
  this->period = BigFraction(period_num, period_denum);
  this->period.DeleteZeros();

  this->integer.reduce();
  this->fractional.reduce();
  this->period.reduce();
}

std::ostream &operator<<(std::ostream &os, const BigNumber &big_num) {
  BigFraction full_fraction =
      big_num.integer + big_num.fractional + big_num.period;

  full_fraction.reduce();

  BigInt integer = full_fraction.GetInteger();
  BigInt fractional = full_fraction.GetFractional();
  BigInt denum = full_fraction.GetDenum();

  integer.DeleteZeros();
  fractional.DeleteZeros();
  denum.DeleteZeros();

  if (fractional.ToDecimalInt() != 0) {
    std::string fractional_part;
    std::string res_fractional_part;

    while (fractional.ToDecimalInt() != 0) {
      fractional = fractional << 1;
      fractional_part += (fractional / denum).ToString();
      fractional = fractional % denum;

      std::vector<int> prefix_function =
          big_num.PrefixFunction(fractional_part);

      for (int i = 0; i < prefix_function.size(); ++i) {
        if (prefix_function[i] != 0 && (i + 1) * 8 == prefix_function[i] * 9) {
          res_fractional_part +=
              fractional_part.substr(0, prefix_function.size() - i - 1);
          res_fractional_part += '(';
          res_fractional_part += fractional_part.substr(
              prefix_function.size() - i - 1, prefix_function[i] / 8);
          res_fractional_part += ')';

          return os;
        }
      }
    }
  }
  return os;
}

std::string BigNumber::ToString() {
  std::string res;
  BigFraction full_fraction = this->integer + this->fractional + this->period;

  full_fraction.reduce();

  BigInt integer = full_fraction.GetInteger();
  BigInt fractional = full_fraction.GetFractional();
  BigInt denum = full_fraction.GetDenum();

  integer.DeleteZeros();
  fractional.DeleteZeros();
  denum.DeleteZeros();

  res += integer.ToString();

  if (fractional.ToDecimalInt() != 0) {
    res += '.';

    std::string fractional_part;
    std::string res_fractional_part;

    while (fractional.ToDecimalInt() != 0) {
      fractional = fractional << 1;
      fractional_part += (fractional / denum).ToString();
      fractional = fractional % denum;

      std::vector<int> prefix_function = this->PrefixFunction(fractional_part);

      for (int i = 0; i < prefix_function.size(); ++i) {
        if (prefix_function[i] != 0 && (i + 1) * 8 == prefix_function[i] * 9) {
          res_fractional_part +=
              fractional_part.substr(0, prefix_function.size() - i - 1);
          res_fractional_part += '(';
          res_fractional_part += fractional_part.substr(
              prefix_function.size() - i - 1, prefix_function[i] / 8);
          res_fractional_part += ')';

          res += res_fractional_part;
          return res;
        }
      }
    }
    res += fractional_part;
  }

  return res;
}

BigInt BigNumber::GetInteger() const {
  BigInt res({0}, 0, this->integer.GetBase());

  res += integer.GetInteger();
  if (this->has_fractional) {
    res += fractional.GetInteger();
  }
  res += period.GetInteger();

  return res;
}

BigInt BigNumber::GetFractional() const {
  BigInt res({0}, 0, this->integer.GetBase());

  res += integer.GetFractional();
  if (this->has_fractional) {
    res += fractional.GetFractional();
  }
  res += period.GetFractional();

  return res;
}

BigNumber BigNumber::Convert(const uint8_t &base) const {
  BigFraction integer = this->integer.Convert(base);
  BigFraction fractional = this->fractional.Convert(base);
  BigFraction period = this->period.Convert(base);

  integer.reduce();
  fractional.reduce();
  period.reduce();

  return BigNumber(integer, fractional, period);
}

std::vector<int> BigNumber::PrefixFunction(std::string s) const {
  std::reverse(s.begin(), s.end());
  int n = (int)s.size();
  std::vector<int> p(n, 0);
  for (int i = 1; i < n; i++) {
    // префикс функция точно не больше этого значения + 1
    int cur = p[i - 1];
    // уменьшаем cur значение, пока новый символ не сматчится
    while (s[i] != s[cur] && cur > 0)
      cur = p[cur - 1];
    // здесь либо s[i] == s[cur], либо cur == 0
    if (s[i] == s[cur])
      p[i] = cur + 1;
  }
  std::reverse(s.begin(), s.end());
  return p;
}
