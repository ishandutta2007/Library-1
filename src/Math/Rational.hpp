#pragma once
#include <string>
#include <limits>
#include <sstream>
#include "src/Math/binary_gcd.hpp"
template <class Int, bool reduction= true> struct Rational {
 Int num, den;
 constexpr Rational(Int num= 0, Int den= 1): num(num), den(den) {
  if (den < 0) num= -num, den= -den;
  if constexpr (reduction) reduce();
 }
 constexpr void reduce() {
  const Int g= binary_gcd(num < 0 ? -num : num, den);
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
  else return den == 0 && r.den == 0 ? num * r.num > 0 : num * r.den == den * r.num;
 }
 constexpr bool operator!=(const Rational &r) const { return !(*this == r); }
 constexpr bool operator<(const Rational &r) const {
  if (den == 0 && r.den == 0) return num < r.num;
  else if (den == 0) return num < 0;
  else if (r.den == 0) return r.num > 0;
  else return num * r.den < den * r.num;
 }
 constexpr bool operator>(const Rational &r) const { return r < *this; }
 constexpr bool operator<=(const Rational &r) const { return !(r < *this); }
 constexpr bool operator>=(const Rational &r) const { return !(*this < r); }
 constexpr explicit operator bool() const { return num != 0; }
 constexpr long double to_fp() const { return (long double)num / den; }
 constexpr Int floor() const {
  if constexpr (reduction) return num < 0 ? -((-num + den - 1) / den) : num / den;
  else {
   const Int n= num < 0 ? -num : num, d= den < 0 ? -den : den;
   return num * den < 0 ? -((n + d - 1) / d) : n / d;
  }
 }
 std::string to_string() const {
  if (!num) return "0";
  std::stringstream ss;
  return ss << num << "/" << den, ss.str();
 }
 friend std::ostream &operator<<(std::ostream &os, const Rational &r) { return os << r.to_string(); }
};
template <class Int> struct std::numeric_limits<Rational<Int, false>> {
 static constexpr Rational<Int, false> max() noexcept { return Rational<Int, false>(1, 0); }
 static constexpr Rational<Int, false> min() noexcept { return Rational<Int, false>(1, std::numeric_limits<Int>::max()); }
 static constexpr Rational<Int, false> lowest() noexcept { return Rational<Int, false>(-1, 0); }
};
template <class Int> struct std::numeric_limits<Rational<Int, true>> {
 static constexpr Rational<Int, true> max() noexcept { return Rational<Int, true>(1, 0); }
 static constexpr Rational<Int, true> min() noexcept { return Rational<Int, true>(1, std::numeric_limits<Int>::max()); }
 static constexpr Rational<Int, true> lowest() noexcept { return Rational<Int, true>(-1, 0); }
};