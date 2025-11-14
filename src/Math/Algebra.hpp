#pragma once
#include "src/Internal/detection_idiom.hpp"
template <class M> struct Algebra {
 using T= typename M::T;
 _DETECT_BOOL(has_zero, decltype(T::o));
 _DETECT_BOOL(has_one, decltype(T::i));
 static inline T zero= has_zero_v<M> ? M::o : T();
 static inline T one= has_one_v<M> ? M::i : T();
 T x;
 Algebra(): x(zero) {}
 Algebra(bool y): x(y ? one : zero) {}
 template <class U, typename= std::enable_if_t<std::is_convertible_v<U, T>>> Algebra(U y): x(y) {}
 Algebra &operator+=(const Algebra &r) { return *this= *this + r; }
 Algebra &operator-=(const Algebra &r) { return *this= *this - r; }
 Algebra &operator*=(const Algebra &r) { return *this= *this * r; }
 Algebra operator+(const Algebra &r) const { return Algebra(M::add(x, r.x)); }
 Algebra operator-(const Algebra &r) const { return Algebra(M::add(x, M::neg(r.x))); }
 Algebra operator*(const Algebra &r) const { return Algebra(M::mul(x, r.x)); }
 Algebra operator-() const { return Algebra(M::neg(x)); }
 bool operator==(const Algebra &r) const { return x == r.x; }
 bool operator!=(const Algebra &r) const { return x != r.x; }
 friend std::istream &operator>>(std::istream &is, Algebra &r) { return is >> r.x, is; }
 friend std::ostream &operator<<(std::ostream &os, const Algebra &r) { return os << r.x; }
};