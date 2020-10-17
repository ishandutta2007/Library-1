/**
 * @title ModInt
 * @category 数学
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <uint64_t mod, uint64_t prim_root = 0>
class ModInt {
 private:
  using u128 = __uint128_t;
  static constexpr uint64_t mul_inv(uint64_t n, int e = 6, uint64_t x = 1) {
    return e == 0 ? x : mul_inv(n, e - 1, x * (2 - x * n));
  }
  static constexpr uint64_t inv = mul_inv(mod, 6, 1);
  static constexpr uint64_t r2 = -u128(mod) % mod;
  static constexpr uint64_t m2 = mod << 1;

 public:
  static constexpr int level = __builtin_ctzll(mod - 1);
  ModInt() = default;
  ~ModInt() = default;
  constexpr ModInt(uint64_t n) : x(init(n)){};
  static constexpr uint64_t modulo() { return mod; }
  static constexpr uint64_t init(uint64_t w) { return reduce(u128(w) * r2); }
  static constexpr uint64_t reduce(const u128 w) {
    return uint64_t(w >> 64) + mod - ((u128(uint64_t(w) * inv) * mod) >> 64);
  }
  static constexpr uint64_t pr_rt() { return prim_root; }
  constexpr ModInt operator-() const {
    ModInt ret;
    return ret.x = (m2 & -(x != 0)) - x, ret;
  }
  constexpr ModInt &operator+=(const ModInt &rhs) {
    return x += rhs.x - m2, x += m2 & -(x >> 63), *this;
  }
  constexpr ModInt &operator-=(const ModInt &rhs) {
    return x -= rhs.x, x += m2 & -(x >> 63), *this;
  }
  constexpr ModInt &operator*=(const ModInt &rhs) {
    return this->x = reduce(u128(this->x) * rhs.x), *this;
  }
  constexpr ModInt &operator/=(const ModInt &rhs) {
    return this->operator*=(rhs.inverse());
  }
  ModInt operator+(const ModInt &rhs) const { return ModInt(*this) += rhs; }
  ModInt operator-(const ModInt &rhs) const { return ModInt(*this) -= rhs; }
  ModInt operator*(const ModInt &rhs) const { return ModInt(*this) *= rhs; }
  ModInt operator/(const ModInt &rhs) const { return ModInt(*this) /= rhs; }
  bool operator==(const ModInt &rhs) const { return x == rhs.x; }
  bool operator!=(const ModInt &rhs) const { return x != rhs.x; }
  uint64_t get() const {
    uint64_t ret = reduce(x) - mod;
    return ret + (mod & -(ret >> 63));
  }
  void set(uint64_t n) const { this->x = n; }
  constexpr ModInt pow(uint64_t k) const {
    ModInt ret = ModInt(1);
    for (ModInt base = *this; k; k >>= 1, base *= base)
      if (k & 1) ret *= base;
    return ret;
  }
  constexpr ModInt inverse() const { return pow(mod - 2); }
  friend std::istream &operator>>(std::istream &is, ModInt &rhs) {
    return is >> rhs.x, rhs.x = init(rhs.x), is;
  }
  friend std::ostream &operator<<(std::ostream &os, const ModInt &rhs) {
    return os << rhs.get();
  }
  uint64_t x;
};