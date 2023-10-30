#pragma once
#include <cstdint>
#include <valarray>
namespace la_internal {
using namespace std;
template <class R> struct Vector: public valarray<R> {
 using valarray<R>::valarray;
};
using u128= __uint128_t;
using u8= uint8_t;
class Ref {
 u128 *ref;
 u8 i;
 bool val;
public:
 Ref(u128 *r, u8 j, bool v): ref(r), i(j), val(v) {}
 ~Ref() {
  if (val ^ ((*ref >> i) & 1)) *ref^= u128(1) << i;
 }
 Ref &operator=(const Ref &r) { return val= r.val, *this; }
 Ref &operator=(bool b) { return val= b, *this; }
 Ref &operator|=(bool b) { return val|= b, *this; }
 Ref &operator&=(bool b) { return val&= b, *this; }
 Ref &operator^=(bool b) { return val^= b, *this; }
 operator bool() const { return val; }
};
template <> class Vector<bool> {
 size_t n;
 valarray<u128> dat;
public:
 Vector(): n(0) {}
 Vector(size_t n): n(n), dat((n + 127) >> 7) {}
 Vector(bool b, size_t n): n(n), dat(-u128(b), (n + 127) >> 7) {}
 Ref operator[](int i) {
  u128 *ref= begin(dat) + (i >> 7);
  u8 j= i & 127;
  bool val= (*ref >> j) & 1;
  return Ref{ref, j, val};
 }
 bool operator[](int i) const { return (dat[i >> 7] >> (i & 127)) & 1; }
 Vector &operator+=(const Vector &r) { return dat^= r.dat, *this; }
 Vector &operator-=(const Vector &r) { return dat^= r.dat, *this; }
 Vector &operator*=(bool b) {
  if (!b) dat= 0;
  return *this;
 }
 Vector operator+(const Vector &r) const { return Vector(*this)+= r; }
 Vector operator-(const Vector &r) const { return Vector(*this)-= r; }
 Vector operator*(bool b) const { return Vector(*this)*= b; }
 size_t size() const { return n; }
 u128 *data() { return begin(dat); }
 friend Vector operator*(bool b, const Vector &r) { return r * b; }
};
}
using la_internal::Vector;