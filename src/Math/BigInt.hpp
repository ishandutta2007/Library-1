#pragma once
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
/**
 * @title 多倍長整数
 * @category 数学
 */

// BEGIN CUT HERE

struct BigInt {
  static constexpr int bdig = 26, base = 1 << bdig;
  bool neg;

 private:
  std::vector<unsigned> dat;
  using ModB = ModInt<4611615649683210241, 11>;
  template <class mod_t>
  static inline void idft(int n, mod_t x[]) {
    static mod_t iW[1 << 20];
    static constexpr std::uint64_t mod = mod_t::modulo();
    static constexpr mod_t G(mod_t::pr_rt());
    static int lim = 0;
    if (lim == 0) iW[0] = 1, lim = 1;
    for (int m = lim; m < n / 2; m *= 2) {
      mod_t idw = G.pow(mod - 1 - (mod - 1) / (4 * m));
      for (int i = 0; i < m; i++) iW[m + i] = iW[i] * idw;
      lim = n / 2;
    }
    for (int m = 1; m < n; m *= 2)
      for (int s = 0, k = 0; s < n; s += 2 * m, ++k)
        for (int i = s, j = s + m; i < s + m; ++i, ++j) {
          mod_t u = x[i], v = x[j];
          x[i] = u + v, x[j] = (u - v) * iW[k];
        }
    mod_t iv(mod - (mod - 1) / n);
    for (int i = 0; i < n; i++) x[i] *= iv;
  }
  template <class mod_t>
  static inline void dft(int n, mod_t x[]) {
    static mod_t W[1 << 20];
    static constexpr std::uint64_t mod = mod_t::modulo();
    static constexpr mod_t G(mod_t::pr_rt());
    static int lim = 0;
    if (lim == 0) W[0] = 1, lim = 1;
    for (int m = lim; m < n / 2; m *= 2) {
      mod_t dw = G.pow((mod - 1) / (4 * m));
      for (int i = 0; i < m; i++) W[m + i] = W[i] * dw;
      lim = n / 2;
    }
    for (int m = n; m >>= 1;)
      for (int s = 0, k = 0; s < n; s += 2 * m, ++k)
        for (int i = s, j = s + m; i < s + m; ++i, ++j) {
          mod_t u = x[i], v = x[j] * W[k];
          x[i] = u + v, x[j] = u - v;
        }
  }
  static inline int get_len(int n) {
    return --n, n |= n >> 1, n |= n >> 2, n |= n >> 4, n |= n >> 8,
           n |= n >> 16, ++n;
  }
  BigInt base_shift_r(int size) const {
    BigInt ret;
    return ret.dat = std::vector<unsigned>(dat.begin() + size, dat.end()), ret;
  }
  BigInt base_shift_l(int size) const {
    BigInt ret = *this;
    return ret.dat.insert(ret.dat.begin(), size, 0), ret;
  }

 public:
  BigInt() : neg(false), dat() {}
  BigInt(long long v) { *this = v; }
  BigInt(const std::string &s) { read(s); }
  void shrink() {
    while (dat.size() && !dat.back()) dat.pop_back();
    if (dat.empty()) neg = false;
  }
  bool is_zero() const { return dat.empty() || (dat.size() == 1 && !dat[0]); }
  bool able_ll() const {
    return dat.size() <= 2
           || (dat.size() == 3 && !(dat.back() >> (63 - 2 * bdig)));
  }
  int ctz() const {
    if (is_zero()) return -1;
    int ret = 0, i;
    for (i = 0; i < (int)dat.size() && !dat[i]; i++) ret += bdig;
    return ret + __builtin_ctz(dat[i]);
  }
  void read(const std::string &s) {
    dat.clear(), neg = s[0] == '-';
    std::vector<short> v;
    for (int i = s.length() - 1; i >= 0 && '0' <= s[i] && s[i] <= '9'; i--)
      v.emplace_back(s[i] - '0');
    for (int rem = 0; !v.empty(); dat.emplace_back(rem), rem = 0) {
      for (int i = v.size() - 1, cur; i >= 0; i--)
        cur = rem * 10 + v[i], v[i] = cur >> bdig, rem = cur & (base - 1);
      while (v.size() && !v.back()) v.pop_back();
    }
  }
  std::string to_str() const {
    if (is_zero()) return "0";
    auto v = dat;
    std::string ret;
    for (short rem = 0; !v.empty(); ret += rem + '0', rem = 0) {
      for (int i = v.size() - 1, cur; i >= 0; i--)
        cur = (rem << bdig) + v[i], v[i] = cur / 10, rem = cur % 10;
      while (v.size() && !v.back()) v.pop_back();
    }
    if (neg) ret += '-';
    std::reverse(ret.begin(), ret.end());
    return ret;
  }
  BigInt &operator=(long long r) {
    neg = false, dat.clear();
    if (r < 0) neg = true, r = -r;
    for (; r; r >>= bdig) dat.emplace_back(r & (base - 1));
    return *this;
  }
  bool operator<(const BigInt &r) const {
    if (neg != r.neg) return neg;
    if (dat.size() != r.dat.size()) return (dat.size() < r.dat.size()) ^ neg;
    for (int i = dat.size() - 1; i >= 0; i--)
      if (dat[i] != r.dat[i]) return (dat[i] < r.dat[i]) ^ neg;
    return false;
  }
  bool operator>(const BigInt &r) const { return r < *this; }
  bool operator<=(const BigInt &r) const { return !(r < *this); }
  bool operator>=(const BigInt &r) const { return !(*this < r); }
  bool operator==(const BigInt &r) const {
    return neg == r.neg && dat == r.dat;
  }
  bool operator!=(const BigInt &r) const { return !(*this == r); }
  BigInt abs() const {
    BigInt ret = *this;
    return ret.neg = false, ret;
  }
  BigInt operator-() const {
    BigInt ret = *this;
    return ret.neg = !ret.neg, ret;
  }
  BigInt operator+(const BigInt &r) const {
    if (neg != r.neg) return *this - (-r);
    BigInt ret = r;
    for (int i = 0, ed = std::max(dat.size(), r.dat.size()), carry = 0;
         i < ed || carry; i++) {
      if (i == (int)ret.dat.size()) ret.dat.emplace_back(0);
      ret.dat[i] += carry + (i < (int)dat.size() ? dat[i] : 0);
      if ((carry = (ret.dat[i] >= base))) ret.dat[i] -= base;
    }
    return ret;
  }
  BigInt operator-(const BigInt &r) const {
    if (neg != r.neg) return *this + (-r);
    if (abs() < r.abs()) return -(r - *this);
    BigInt ret = *this;
    for (unsigned i = 0, carry = 0; i < r.dat.size() || carry; i++) {
      ret.dat[i] += base - (carry + (i < r.dat.size() ? r.dat[i] : 0));
      if (!(carry = (ret.dat[i] < base))) ret.dat[i] -= base;
    }
    return ret.shrink(), ret;
  }
  BigInt &operator+=(const BigInt &r) { return *this = *this + r; }
  BigInt &operator-=(const BigInt &r) { return *this = *this - r; }
  BigInt &operator*=(long long r) {
    if (r < 0) neg = !neg, r = -r;
    for (long long i = 0, carry = 0; i < (long long)dat.size() || carry; i++) {
      if (i == (long long)dat.size()) dat.emplace_back(0);
      __uint128_t cur = (__uint128_t)r * dat[i] + carry;
      carry = cur >> bdig, dat[i] = cur & (base - 1);
    }
    return shrink(), *this;
  }
  BigInt &operator/=(long long r) {
    if (r < 0) neg = !neg, r = -r;
    for (__int128_t i = dat.size() - 1, cur, rem = 0; i >= 0; i--)
      cur = dat[i] + (rem << bdig), dat[i] = cur / r, rem = cur % r;
    return shrink(), *this;
  }
  long long operator%(long long r) const {
    long long ret = 0;
    for (int i = dat.size(); i;)
      ret = (dat[--i] + (__uint128_t(ret) << bdig)) % r;
    return ret;
  }
  BigInt operator*(long long r) const { return BigInt(*this) *= r; }
  BigInt operator/(long long r) const { return BigInt(*this) /= r; }
  BigInt operator*(const BigInt &r) const {
    static ModB f[1 << 20], g[1 << 20];
    static long long h[1 << 20];
    int n = dat.size(), m = r.dat.size(), sz = n + m - 1;
    if (n <= 8 || m <= 8) {
      std::fill_n(h, sz, 0);
      for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) h[i + j] += (long long)dat[i] * r.dat[j];
    } else {
      for (int i = 0; i < n; i++) f[i] = dat[i];
      for (int i = 0; i < m; i++) g[i] = r.dat[i];
      int len = get_len(sz);
      std::fill(f + n, f + len, 0), std::fill(g + m, g + len, 0);
      dft(len, f), dft(len, g);
      for (int i = 0; i < len; i++) f[i] *= g[i];
      idft(len, f);
      for (int i = 0; i < sz; i++) h[i] = f[i].val();
    }
    BigInt ret;
    for (long long i = 0, carry = 0, cur; i < sz || carry; i++)
      cur = carry + (i < sz ? h[i] : 0), carry = cur >> bdig,
      ret.dat.emplace_back(cur & (base - 1));
    return ret.neg = neg ^ r.neg, ret;
  }
  BigInt &operator*=(const BigInt &r) { return *this = *this * r; }
  BigInt operator/(const BigInt &r) const {
    if (r.dat.size() == 1 && r.dat.back() == 1) return *this;
    if (r.able_ll()) return *this / r.convert_ll();
    static ModB f[1 << 20], g[1 << 20];
    int pb = dat.size(), qb = r.dat.size(), prec = std::max(pb - qb, 1),
        lim = std::min(prec, 3), rlim = std::min(qb, 6);
    BigInt x, prev, rr = r.base_shift_r(qb - rlim), c;
    x.dat.resize(lim + 1, 0), x.dat.back() = 1;
    c.dat.resize(rlim + lim + 1, 0), c.dat.back() = 2;
    while (x != prev) prev = x, x *= c - rr * x, x = x.base_shift_r(lim + rlim);
    if (lim != prec) {
      for (prev.dat.clear(); x != prev;) {
        prev = x;
        int n = x.dat.size(), m = rr.dat.size(), sz = n + m - 1,
            len1 = get_len(sz), len2 = get_len(rlim + lim + n),
            nlim = std::min(lim * 2 + 1, prec),
            nrlim = std::min(rlim * 2 + 1, qb);
        for (int i = 0; i < n; i++) g[i] = x.dat[i];
        for (int i = 0; i < m; i++) f[i] = rr.dat[i];
        std::fill(g + n, g + len2, 0), std::fill(f + m, f + len1, 0);
        dft(len1, f), dft(len2, g);
        for (int i = 0; i < len1; i++) f[i] *= g[i];
        idft(len1, f), x.dat.clear();
        for (long long i = 0, carry = 0, cur; i < sz || carry; i++)
          cur = carry + (i < sz ? f[i].val() : 0), carry = cur >> bdig,
          x.dat.emplace_back(cur & (base - 1));
        x = c - x, m = x.dat.size(), sz = n + m - 1;
        for (int i = 0; i < m; i++) f[i] = x.dat[i];
        std::fill(f + m, f + len2, 0), dft(len2, f);
        for (int i = 0; i < len2; i++) f[i] *= g[i];
        idft(len2, f), x.dat.clear();
        for (long long i = 0, carry = 0, cur; i < sz || carry; i++) {
          cur = carry + (i < sz ? f[i].val() : 0), carry = cur >> bdig;
          if (i >= 2 * lim - nlim + rlim) x.dat.emplace_back(cur & (base - 1));
        }
        if (nrlim != rlim) rr = r.base_shift_r(qb - nrlim);
        if (nrlim != rlim || nlim != lim)
          c.dat.back() = 0, c.dat.resize(nrlim + nlim + 1, 0), c.dat.back() = 2;
        lim = nlim, rlim = nrlim;
      }
    }
    x *= this->abs(), x = x.base_shift_r(pb + (pb == qb));
    if (this->abs() >= (x + 1) * r) x += 1;
    return x.neg = neg ^ r.neg, x.shrink(), x;
  }
  BigInt &operator/=(const BigInt &r) { return *this = *this / r; }
  BigInt &operator%=(const BigInt &r) { return *this -= (*this / r) * r; }
  BigInt operator%(const BigInt &r) const { return BigInt(*this) %= r; }
  BigInt &operator>>=(unsigned size) {
    if (dat.size() * bdig <= size) return *this = 0;
    unsigned i = 0, j = size / bdig, k = size % bdig, mask = (1 << k) - 1;
    for (; j + 1 < dat.size(); i++, j++)
      dat[i] = (dat[j] >> k) | ((dat[j + 1] & mask) << (bdig - k));
    return dat[i] = (dat[j] >> k), dat.resize(i + 1), shrink(), *this;
  }
  BigInt &operator<<=(unsigned size) {
    if (is_zero()) return *this;
    int i = dat.size(), k = size % bdig;
    for (dat.emplace_back(0); i > 0; i--)
      dat[i] = ((dat[i] << k) & (base - 1)) | (dat[i - 1] >> (bdig - k));
    dat[0] = (dat[0] << k) & (base - 1);
    return shrink(), dat.insert(dat.begin(), size / bdig, 0), *this;
  }
  BigInt operator>>(unsigned size) const { return BigInt(*this) >>= size; }
  BigInt operator<<(unsigned size) const { return BigInt(*this) <<= size; }
  long long convert_ll() const {
    assert(able_ll());
    long long ret = 0;
    for (int i = dat.size() - 1; i >= 0; i--) ret = (ret << bdig) + dat[i];
    return neg ? -ret : ret;
  }
  friend std::istream &operator>>(std::istream &is, BigInt &v) {
    std::string s;
    return is >> s, v.read(s), is;
  }
  friend std::ostream &operator<<(std::ostream &os, const BigInt &v) {
    return os << v.to_str(), os;
  }
};

BigInt gcd(const BigInt &a, const BigInt &b) {
  assert(!a.neg && !b.neg);
  if (a.is_zero()) return b;
  if (b.is_zero()) return a;
  int y = a.ctz(), z = b.ctz();
  BigInt x[2] = {a, b};
  x[0] >>= y, x[1] >>= z;
  bool id = 0;
  while (true) {
    if (x[id] < x[!id]) id = !id;
    if ((x[id] -= x[!id]).is_zero()) break;
    x[id] >>= x[id].ctz();
  }
  return x[!id] << std::min(y, z);
}

BigInt lcm(const BigInt &a, const BigInt &b) { return (a / gcd(a, b)) * b; }