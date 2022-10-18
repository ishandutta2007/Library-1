#pragma once
#include <bits/stdc++.h>
/**
 * @title Nimber
 * @category 数学
 * @see https://en.wikipedia.org/wiki/Nimber
 * @see https://natsugiri.hatenablog.com/entry/2020/03/29/073605
 */

// BEGIN CUT HERE
class Nimber {
  using u64 = std::uint64_t;
  using u32 = std::uint32_t;
  using u16 = std::uint16_t;
  static inline const auto pw = [](u32 x, u32 y) {
    constexpr u16 f2n[16] = {
        0x0001u, 0x2827u, 0x392bu, 0x8000u, 0x20fdu, 0x4d1du, 0xde4au, 0x0a17u,
        0x3464u, 0xe3a9u, 0x6d8du, 0x34bcu, 0xa921u, 0xa173u, 0x0ebcu, 0x0e69u};
    std::array<u16, 65536> pw = {1};
    for (int i = 1; i < 65535; ++i)
      pw[i] = (pw[i - 1] << 1) ^ (0x1681fu & (-(pw[i - 1] >= 0x8000u)));
    for (int i = 1; i < 65535; pw[i++] = y)
      for (x = pw[i], y = 0; x; x &= x - 1) y ^= f2n[__builtin_ctz(x)];
    return pw[65535] = 1, pw;
  }(0, 0);
  static inline const auto ln = []() {
    std::array<u16, 65536> ln = {};
    for (int i = 1; i < 65535; i++) ln[pw[i]] = i;
    return ln;
  }();
  template <u16 h = 3>
  static inline u16 half(u16 A) {
    return A ? pw[(ln[A] + h) % 65535] : 0;
  }
  template <u16 h = 0>
  static inline u16 mul(u16 A, u16 B) {
    return A && B ? pw[(ln[A] + ln[B] + h) % 65535] : 0;
  }
  template <u16 h = 0>
  static inline u16 mul(u16 A, u16 B, u16 C) {
    return A && B && C ? pw[(ln[A] + ln[B] + ln[C] + h) % 65535] : 0;
  }
  static inline u16 inv(u16 A) { return assert(A), pw[65535 - ln[A]]; }
  static inline u16 sqrt(u16 A) {
    return A ? pw[u16((65537 * u32(ln[A])) >> 1)] : 0;
  }
  static inline u64 mul(u64 A, u64 B) {
    u16 a0 = u16(A), a1 = u16(A >> 16), a2 = u16(A >> 32), a3 = A >> 48;
    u16 b0 = u16(B), b1 = u16(B >> 16), b2 = u16(B >> 32), b3 = B >> 48;
    u16 x0 = a1 ^ a0, x1 = a3 ^ a2, y0 = b1 ^ b0, y1 = b3 ^ b2;
    u16 c0 = mul(a0, b0), c1 = mul(x0, y0) ^ c0, c2 = mul<0>(a2 ^ a0, b2 ^ b0);
    u16 c3 = mul<0>(x0 ^ x1, y0 ^ y1) ^ c2 ^ c1;
    c2 ^= (c0 ^= mul<3>(a1, b1)) ^ mul<3>(u16(a3 ^ a1), u16(b3 ^ b1));
    c1 ^= mul<6>(a3, b3) ^ mul<3>(x1, y1);
    c0 ^= mul<6>(a2, b2) ^ mul<6>(x1, y1);
    return (u64(c3) << 48) | (u64(c2) << 32) | (u32(c1) << 16) | c0;
  }
  static inline u64 inv(u64 A) {
    u16 a0 = u16(A), a1 = u16(A >> 16), a2 = u16(A >> 32), a3 = A >> 48;
    u16 x = a2 ^ a3, y = a1 ^ a3, w = a0 ^ a2, v = a0 ^ a1;
    u16 b3 = mul(a1, a2, a1 ^ x), b2 = mul(a0, a2, a0 ^ x),
        b1 = mul(a0, a1, a0 ^ y), b0 = mul(a0, v, w), t = mul<3>(w, x, x);
    b0 ^= b1 ^ b2, b1 ^= b3, b2 ^= b3, b0 ^= b3 ^= mul(a0, a0, a3);
    b1 ^= t ^ mul<3>(a1, y, y), b0 ^= t ^ mul<3>(v, y, y);
    b3 ^= t = mul<3>(a1, a3, y) ^ mul<3>(a2, x, x);
    b2 ^= t ^ mul<3>(a0, a3, a3) ^ mul<3>(a1, a1, a2), b3 ^= mul<6>(a3, a3, x);
    b2 ^= mul<6>(a3, x, x), b1 ^= mul<6>(a3, a3, y ^ w), b0 ^= mul<6>(y, x, x);
    b2 ^= mul<9>(a3, a3, a3), b0 ^= mul<9>(a3, a3, y);
    t = mul<6>(x, b3) ^ mul<6>(a3, b2) ^ mul<3>(a1, b1) ^ mul(a0, b0);
    return t = inv(t), (u64(mul(b3, t)) << 48) | (u64(mul(b2, t)) << 32) |
                           (u32(mul(b1, t)) << 16) | mul(b0, t);
  }
  static inline u64 square(u64 A) {
    u16 a0 = u16(A), a1 = u16(A >> 16), a2 = u16(A >> 32), a3 = A >> 48;
    a3 = mul(a3, a3), a2 = mul(a2, a2), a1 = mul(a1, a1), a0 = mul(a0, a0);
    a0 ^= half(a1) ^ half<6>(a3), a2 ^= half(a3), a1 ^= half(a3 ^ a2);
    return (u64(a3) << 48) | (u64(a2) << 32) | (u32(a1) << 16) | a0;
  }
  u64 x;

 public:
  Nimber(u64 x_ = 0) : x(x_) {}
  Nimber &operator+=(const Nimber &r) { return x ^= r.x, *this; }
  Nimber &operator-=(const Nimber &r) { return x ^= r.x, *this; }
  Nimber &operator*=(const Nimber &r) { return x = mul(x, r.x), *this; }
  Nimber &operator/=(const Nimber &r) { return x = mul(x, inv(r.x)), *this; }
  Nimber operator+(const Nimber &r) const { return Nimber(x ^ r.x); }
  Nimber operator-(const Nimber &r) const { return Nimber(x ^ r.x); }
  Nimber operator*(const Nimber &r) const { return Nimber(mul(x, r.x)); }
  Nimber operator/(const Nimber &r) const { return Nimber(mul(x, inv(r.x))); }
  Nimber inv() const { return Nimber(inv(x)); }
  Nimber square() const { return Nimber(square(x)); }
  Nimber sqrt() const {
    u16 a0 = u16(x), a1 = u16(x >> 16), a2 = u16(x >> 32), a3 = x >> 48;
    a1 ^= half(a3 ^ a2), a2 ^= half(a3), a0 ^= half(a1) ^ half<6>(a3);
    return Nimber((u64(sqrt(a3)) << 48) | (u64(sqrt(a2)) << 32) |
                  (u32(sqrt(a1)) << 16) | sqrt(a0));
  }
  u64 val() const { return x; }
  Nimber pow(u64 k) const {
    for (u64 ret = 1, b = x;; b = square(b))
      if (k & 1 ? ret = mul(ret, b) : 0; !(k >>= 1)) return Nimber(ret);
  }
  bool operator==(const Nimber &r) const { return x == r.x; }
  bool operator!=(const Nimber &r) const { return x != r.x; }
  bool operator<(const Nimber &r) const { return x < r.x; }
  bool operator>(const Nimber &r) const { return x > r.x; }
  bool operator<=(const Nimber &r) const { return x <= r.x; }
  bool operator>=(const Nimber &r) const { return x >= r.x; }
  friend std::ostream &operator<<(std::ostream &os, const Nimber &r) {
    return os << r.x;
  }
  friend std::istream &operator>>(std::istream &is, Nimber &r) {
    return is >> r.x, is;
  }
};