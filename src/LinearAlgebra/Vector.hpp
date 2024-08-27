#pragma once
#include <cstdint>
#include <iostream>
#include <valarray>
namespace _la_internal {
using namespace std;
template <class R> class Matrix;
template <class R> class Vector {
 valarray<R> dat;
public:
 Vector()= default;
 Vector(size_t n): dat(n) {}
 Vector(size_t n, const R &v): dat(v, n) {}
 Vector(const initializer_list<R> &v): dat(v) {}
 R &operator[](int i) { return dat[i]; }
 const R &operator[](int i) const { return dat[i]; }
 bool operator==(const Vector &r) const {
  if (dat.size() != r.dat.size()) return false;
  for (int i= dat.size(); i--;)
   if (dat[i] != r.dat[i]) return false;
  return true;
 }
 bool operator!=(const Vector &r) const { return !(*this == r); }
 Vector &operator+=(const Vector &r) { return dat+= r.dat, *this; }
 Vector &operator-=(const Vector &r) { return dat-= r.dat, *this; }
 Vector &operator*=(const R &r) { return dat*= r, *this; }
 Vector operator+(const Vector &r) const { return Vector(*this)+= r; }
 Vector operator-(const Vector &r) const { return Vector(*this)-= r; }
 Vector operator*(const R &r) const { return Vector(*this)*= r; }
 size_t size() const { return dat.size(); }
 R *data() { return begin(dat); }
 friend R dot(const Vector<R> &a, const Vector<R> &b) { return assert(a.size() == b.size()), (a.dat * b.dat).sum(); }
};
using u128= __uint128_t;
using u8= uint8_t;
using u64= uint64_t;
class Ref {
 u128 *ref;
 u8 i;
public:
 Ref(u128 *ref, u8 i): ref(ref), i(i) {}
 Ref &operator=(const Ref &r) { return *this= bool(r); }
 Ref &operator=(bool b) { return *ref&= ~(u128(1) << i), *ref|= u128(b) << i, *this; }
 Ref &operator|=(bool b) { return *ref|= u128(b) << i, *this; }
 Ref &operator&=(bool b) { return *ref&= ~(u128(!b) << i), *this; }
 Ref &operator^=(bool b) { return *ref^= u128(b) << i, *this; }
 operator bool() const { return (*ref >> i) & 1; }
};
template <> class Vector<bool> {
 size_t n;
 valarray<u128> dat;
 friend class Matrix<bool>;
public:
 Vector(): n(0) {}
 Vector(size_t n): n(n), dat((n + 127) >> 7) {}
 Vector(size_t n, bool b): n(n), dat(-u128(b), (n + 127) >> 7) {
  if (int k= n & 127; k) dat[dat.size() - 1]&= (u128(1) << k) - 1;
 }
 Vector(const initializer_list<bool> &v): n(v.size()), dat((n + 127) >> 7) {
  int i= 0;
  for (bool b: v) dat[i >> 7]|= u128(b) << (i & 127), ++i;
 }
 Ref operator[](int i) { return {begin(dat) + (i >> 7), u8(i & 127)}; }
 bool operator[](int i) const { return (dat[i >> 7] >> (i & 127)) & 1; }
 bool operator==(const Vector &r) const {
  if (dat.size() != r.dat.size()) return false;
  for (int i= dat.size(); i--;)
   if (dat[i] != r.dat[i]) return false;
  return true;
 }
 bool operator!=(const Vector &r) const { return !(*this == r); }
 Vector &operator+=(const Vector &r) { return dat^= r.dat, *this; }
 Vector &operator-=(const Vector &r) { return dat^= r.dat, *this; }
 Vector &operator*=(bool b) { return dat*= b, *this; }
 Vector operator+(const Vector &r) const { return Vector(*this)+= r; }
 Vector operator-(const Vector &r) const { return Vector(*this)-= r; }
 Vector operator*(bool b) const { return Vector(*this)*= b; }
 size_t size() const { return n; }
 u128 *data() { return begin(dat); }
 friend bool dot(const Vector<bool> &a, const Vector<bool> &b) {
  assert(a.size() == b.size());
  u128 v= 0;
  for (int i= a.size(); i--;) v^= a.dat[i] & b.dat[i];
  return __builtin_parityll(v >> 64) ^ __builtin_parityll(u64(v));
 }
};
template <class R> Vector<R> operator*(const R &r, const Vector<R> &v) { return v * r; }
template <class R> ostream &operator<<(ostream &os, const Vector<R> &v) {
 os << '[';
 for (int _= 0, __= v.size(); _ < __; ++_) os << (_ ? ", " : "") << v[_];
 return os << ']';
}
}
using _la_internal::Vector;