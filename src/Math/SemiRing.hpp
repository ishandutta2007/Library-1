#pragma once
#include <bits/stdc++.h>
/**
 * @title 半環
 * @category 数学
 * 行列に載せる
 */

// verify用
// https://atcoder.jp/contests/abc009/tasks/abc009_4
// https://atcoder.jp/contests/ddcc2020-final/tasks/ddcc2020_final_b

// BEGIN CUT HERE

template <typename T, T (*o)(), T (*i)(), T (*add)(T, T), T (*mul)(T, T)>
struct SemiRing {
  T x;
  SemiRing() : x(o()) {}
  SemiRing(T y) : x(y) {}
  static T O() { return o(); }
  static T I() { return i(); }
  SemiRing &operator+=(const SemiRing &r) {
    return x == o() ? *this = r : r.x == o() ? *this : *this = add(x, r.x);
  }
  SemiRing &operator*=(const SemiRing &r) {
    if (x == o() || r.x == o()) return *this = o();
    return x == i() ? *this = r : r.x == i() ? *this : *this = mul(x, r.x);
  }
  SemiRing operator+(const SemiRing &r) const { return SemiRing(*this) += r; }
  SemiRing operator*(const SemiRing &r) const { return SemiRing(*this) *= r; }
  bool operator==(const SemiRing &r) const { return x == r.x; }
  bool operator!=(const SemiRing &r) const { return x != r.x; }
  friend std::istream &operator>>(std::istream &is, SemiRing &r) {
    return is >> r.x, is;
  }
  friend std::ostream &operator<<(std::ostream &os, const SemiRing &r) {
    return os << r.x;
  }
};

template <class T>
struct MinPlus {
  static T o() { return std::numeric_limits<T>::max(); }
  static T i() { return T(0); }
  static T add(T vl, T vr) { return std::min(vl, vr); }
  static T mul(T vl, T vr) { return vl + vr; }
};
template <class T>
using MinPlusRig = SemiRing<T, MinPlus<T>::o, MinPlus<T>::i, MinPlus<T>::add,
                            MinPlus<T>::mul>;

template <class T>
struct MaxPlus {
  static T o() { return std::numeric_limits<T>::min(); }
  static T i() { return T(0); }
  static T add(T vl, T vr) { return std::max(vl, vr); }
  static T mul(T vl, T vr) { return vl + vr; }
};
template <class T>
using MaxPlusRig = SemiRing<T, MaxPlus<T>::o, MaxPlus<T>::i, MaxPlus<T>::add,
                            MaxPlus<T>::mul>;

template <class T>
struct BitwiseOrAnd {
  static T o() { return 0; }
  static T i() { return T(-1); }
  static T add(T vl, T vr) { return vl | vr; }
  static T mul(T vl, T vr) { return vl & vr; }
};
template <class T = unsigned long long>
using BitwiseOrAndRig = SemiRing<T, BitwiseOrAnd<T>::o, BitwiseOrAnd<T>::i,
                                 BitwiseOrAnd<T>::add, BitwiseOrAnd<T>::mul>;

template <class T>
struct BitwiseXorAnd {
  static T o() { return 0; }
  static T i() { return T(-1); }
  static T add(T vl, T vr) { return vl ^ vr; }
  static T mul(T vl, T vr) { return vl & vr; }
};
template <class T = unsigned long long>
using BitwiseXorAndRig = SemiRing<T, BitwiseXorAnd<T>::o, BitwiseXorAnd<T>::i,
                                  BitwiseXorAnd<T>::add, BitwiseXorAnd<T>::mul>;
