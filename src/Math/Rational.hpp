#pragma once
#include <string>
#include <limits>
#include <sstream>
#include "src/Math/binary_gcd.hpp"
template <class Int, bool reduction= true> struct Rational {
 Int num, den;
 constexpr Rational(): num(0), den(1) {}
 constexpr Rational(Int n, Int d= 1): num(n), den(d) {
  if (den < 0) num= -num, den= -den;
  if constexpr (reduction) reduce(num, den);
 }
 constexpr Rational(const std::string &str) {
  auto it= str.find("/");
  if (it == std::string::npos) num= std::stoi(str), den= 1;
  else num= std::stoi(str.substr(0, it)), den= std::stoi(str.substr(it + 1));
  if constexpr (reduction) reduce(num, den);
 }
 static constexpr void reduce(Int &a, Int &b) {
  const Int g= binary_gcd(a < 0 ? -a : a, b);
  a/= g, b/= g;
 }
 static constexpr Rational raw(Int n, Int d) {
  Rational ret;
  return ret.num= n, ret.den= d, ret;
 }
 constexpr Rational operator-() const { return raw(-num, den); }
 constexpr Rational operator+(const Rational &r) const { return Rational(num * r.den + den * r.num, den * r.den); }
 constexpr Rational operator-(const Rational &r) const { return Rational(num * r.den - den * r.num, den * r.den); }
 constexpr Rational operator*(const Rational &r) const {
  if constexpr (reduction) {
   Int ln= num, ld= den, rn= r.num, rd= r.den;
   return reduce(ln, rd), reduce(rn, ld), raw(ln * rn, ld * rd);
  } else return Rational(num * r.num, den * r.den);
 }
 constexpr Rational operator/(const Rational &r) const {
  if constexpr (reduction) {
   Int ln= num, ld= den, rn= r.num, rd= r.den;
   if (rn < 0) rd= -rd, rn= -rn;
   return reduce(ln, rn), reduce(rd, ld), raw(ln * rd, ld * rn);
  } else return Rational(num * r.den, den * r.num);
 }
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
 constexpr explicit operator long double() const { return to_fp(); }
 constexpr explicit operator double() const { return to_fp(); }
 constexpr explicit operator float() const { return to_fp(); }
 constexpr Int floor() const { return num < 0 ? -((-num + den - 1) / den) : num / den; }
 constexpr Int ceil() const { return num < 0 ? -(-num / den) : (num + den - 1) / den; }
 constexpr Rational abs() const { return raw(num < 0 ? -num : num, den); }
 constexpr friend Int floor(const Rational &r) { return r.floor(); }
 constexpr friend Int ceil(const Rational &r) { return r.ceil(); }
 constexpr friend Rational abs(const Rational &r) { return r.abs(); }
 std::string to_string() const {
  if (!num) return "0";
  std::stringstream ss;
  if (den == 1) return ss << num, ss.str();
  return ss << num << "/" << den, ss.str();
 }
 friend std::istream &operator>>(std::istream &is, Rational &r) {
  std::string s;
  if (is >> s; s != "") r= Rational(s);
  return is;
 }
 friend std::ostream &operator<<(std::ostream &os, const Rational &r) { return os << r.to_string(); }
};
template <class Int, bool reduction> struct std::numeric_limits<Rational<Int, reduction>> {
 static constexpr Rational<Int, reduction> max() noexcept { return Rational<Int, reduction>(1, 0); }
 static constexpr Rational<Int, reduction> min() noexcept { return Rational<Int, reduction>(1, std::numeric_limits<Int>::max()); }
 static constexpr Rational<Int, reduction> lowest() noexcept { return Rational<Int, reduction>(-1, 0); }
};