#include "BigFraction.h"
#include "Bigint.h"

BigFraction::BigFraction(const BigInt &num, const BigInt &denum) {
  this->num = num;
  this->denum = denum;
}

BigFraction::BigFraction(const BigInt &num) {
  this->num = num;
  this->denum = 1;
}

uint8_t BigFraction::GetBase() const { return this->num.GetBase(); }

BigInt BigFraction::GetDenum() const { return this->denum; }
BigInt BigFraction::GetNum() const { return this->num; }

BigInt BigFraction::GetInteger() const {
  BigInt res = this->num.Abs() / this->denum.Abs();
  res.DeleteZeros();

  return res;
}

BigInt BigFraction::GetFractional() const {
  BigInt res = this->num.Abs() % this->denum.Abs();
  res.DeleteZeros();

  return res;
}

std::ostream &operator<<(std::ostream &os, const BigFraction &big_frac) {
  os << big_frac.GetInteger() << ' ' << big_frac.GetFractional() << "/"
     << big_frac.GetDenum();
  return os;
}

void BigFraction::DeleteZeros() {
  this->num.DeleteZeros();
  this->denum.DeleteZeros();
}

void BigFraction::reduce() {
  bool sign = this->num.GetSign() ^ this->denum.GetSign();

  this->num = this->num.Abs();
  this->denum = this->denum.Abs();
  BigInt gcd_ = gcd(this->num, this->denum);

  this->num = this->num / gcd_;
  this->num.DeleteZeros();
  if (sign) {
    this->num = -this->num;
  }
  this->denum = this->denum / gcd_;
  this->denum.DeleteZeros();
}

BigFraction BigFraction::Convert(const uint8_t &base) const {

  BigInt num = this->num.Convert(base);
  BigInt denum = this->denum.Convert(base);

  num.DeleteZeros();
  denum.DeleteZeros();

  return BigFraction(num, denum);
}

BigFraction operator+(const BigFraction &first_, const BigFraction &second_) {
  BigInt num = first_.GetNum() * second_.GetDenum() +
               second_.GetNum() * first_.GetDenum();

  BigInt denum = second_.GetDenum() * first_.GetDenum();

  num.DeleteZeros();
  denum.DeleteZeros();

  BigFraction res(num, denum);

  res.reduce();

  return res;
}
