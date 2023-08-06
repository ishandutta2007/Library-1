#pragma once
#include <string>
#include "src/Math/binary_gcd.hpp"
template <class Int, bool reduction= true> struct Rational {
 Int num, den;
 constexpr Rational(Int num= 0, Int den= 1): num(num), den(den) {
  if constexpr (reduction) reduce();
 }
 constexpr void reduce() {
  if (den < 0) num= -num, den= -den;
  const Int g= binary_gcd(num > -num ? num : -num, den);
  num/= g, den/= g;
 }
 constexpr Rational operator-() const { return Rational(-num, den); }
 constexpr Rational operator+(const Rational &r) const { return Rational(num * r.den + den * r.num, den * r.den); }
 constexpr Rational operator-(const Rational &r) const { return Rational(num * r.den - den * r.num, den * r.den); }
 constexpr Rational operator*(const Rational &r) const { return Rational(num * r.num, den * r.den); }
 constexpr Rational operator/(const Rational &r) const { return Rational(num * r.den, den * r.num); }
 Rational &operator+=(const Rational &r) { return *this= *this + r; }
 Rational &operator-=(const Rational &r) { return *this= *this - r; }
 Rational &operator*=(const Rational &r) { return *this= *this * r; }
 Rational &operator/=(const Rational &r) { return *this= *this / r; }
 constexpr bool operator==(const Rational &r) const {
  if constexpr (reduction) return num == r.num && den == r.den;
  else return num * r.den == den * r.num;
 }
 constexpr bool operator!=(const Rational &r) const {
  if constexpr (reduction) return num != r.num || den != r.den;
  else return num * r.den != den * r.num;
 }
 constexpr bool operator<(const Rational &r) const { return num * r.den < den * r.num; }
 constexpr bool operator>(const Rational &r) const { return num * r.den > den * r.num; }
 constexpr bool operator<=(const Rational &r) const { return num * r.den <= den * r.num; }
 constexpr bool operator>=(const Rational &r) const { return num * r.den >= den * r.num; }
 constexpr explicit operator bool() const { return num != 0; }
 constexpr long double to_fp() const { return (long double)num / den; }
 constexpr Int floor() const { return num / den - (num < 0 && num % den); }
 std::string to_string() const { return num ? std::to_string(num) + "/" + std::to_string(den) : "0"; }
 friend std::ostream &operator<<(std::ostream &os, const Rational &r) { return os << r.to_string(); }
};