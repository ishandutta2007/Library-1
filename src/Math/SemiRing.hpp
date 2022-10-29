#pragma once
#include <bits/stdc++.h>
/**
 * @title 半環
 * @category 数学
 * 行列に載せる
 */

// verify用
// https://atcoder.jp/contests/abc009/tasks/abc009_4 (BitwiseXorAnd)
// https://atcoder.jp/contests/ddcc2020-final/tasks/ddcc2020_final_b
// (max+の一次関数)

// BEGIN CUT HERE

template <typename T, T (*o)(), T (*i)(), T (*add)(T, T), T (*mul)(T, T)>
struct SemiRing {
  T x;
  SemiRing() : x(o()) {}
  SemiRing(bool y) : x(y ? i() : o()) {}
  template <class U,
            std::enable_if_t<std::is_convertible_v<U, T>, nullptr_t> = nullptr>
  SemiRing(U y) : x((T)y) {}
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
template <class M>
using Rig = SemiRing<typename M::T, M::o, M::i, M::add, M::mul>;

template <class Arith>
struct MinPlus {
  using T = Arith;
  static T o() { return std::numeric_limits<T>::max(); }
  static T i() { return T(0); }
  static T add(T vl, T vr) { return std::min(vl, vr); }
  static T mul(T vl, T vr) { return vl + vr; }
};
template <class T>
using MinPlusRig = Rig<MinPlus<T>>;

template <class Arith>
struct MaxPlus {
  using T = Arith;
  static T o() { return std::numeric_limits<T>::min(); }
  static T i() { return T(0); }
  static T add(T vl, T vr) { return std::max(vl, vr); }
  static T mul(T vl, T vr) { return vl + vr; }
};
template <class T>
using MaxPlusRig = Rig<MaxPlus<T>>;

template <class Arith>
struct MinMax {
  using T = Arith;
  static T o() { return std::numeric_limits<T>::max(); }
  static T i() { return std::numeric_limits<T>::min(); }
  static T add(T vl, T vr) { return std::min(vl, vr); }
  static T mul(T vl, T vr) { return std::max(vl, vr); }
};
template <class T>
using MinMaxRig = Rig<MinMax<T>>;

template <class Uint>
struct BitwiseOrAnd {
  using T = Uint;
  static T o() { return 0; }
  static T i() { return T(-1); }
  static T add(T vl, T vr) { return vl | vr; }
  static T mul(T vl, T vr) { return vl & vr; }
};
template <class T = unsigned long long>
using BitwiseOrAndRig = Rig<BitwiseOrAnd<T>>;

template <class Uint>
struct BitwiseXorAnd {
  using T = Uint;
  static T o() { return 0; }
  static T i() { return T(-1); }
  static T add(T vl, T vr) { return vl ^ vr; }
  static T mul(T vl, T vr) { return vl & vr; }
};
template <class T = unsigned long long>
using BitwiseXorAndRig = Rig<BitwiseXorAnd<T>>;
