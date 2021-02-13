#pragma once
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
/**
 * @title 多倍長整数
 * @category 数学
 */

// BEGIN CUT HERE

struct BigInt {
  static constexpr unsigned base = 10000000, bdig = 7;
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

 public:
  BigInt() : neg(false), dat() {}
  BigInt(long long v) { *this = v; }
  BigInt(const std::string &s) { read(s); }
  void read(const std::string &s) {
    neg = false, dat.clear();
    int pos = 0;
    for (; pos < (int)s.size() && (s[pos] == '-' || s[pos] == '+'); ++pos)
      if (s[pos] == '-') neg = !neg;
    for (long long i = s.size() - 1, x = 0; i >= pos; i -= bdig, x = 0) {
      for (int j = std::max<int>(pos, i - bdig + 1); j <= i; j++)
        x = x * 10 + s[j] - '0';
      dat.push_back(x);
    }
    shrink();
  }
  std::string to_str() const {
    std::stringstream ss;
    if (neg) ss << '-';
    ss << (dat.empty() ? 0 : dat.back());
    for (long long i = dat.size() - 2; i >= 0; --i)
      ss << std::setw(bdig) << std::setfill('0') << dat[i];
    std::string ret;
    return ss >> ret, ret;
  }
  void shrink() {
    while (!dat.empty() && !dat.back()) dat.pop_back();
    if (dat.empty()) neg = false;
  }
  bool is_zero() const { return dat.empty() || (dat.size() == 1 && !dat[0]); }
  BigInt &operator=(long long r) {
    neg = false, dat.clear();
    if (r < 0) neg = true, r = -r;
    for (; r; r /= base) dat.emplace_back(r % base);
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
    for (unsigned i = 0, ed = std::max(dat.size(), r.dat.size()), carry = 0;
         i < ed || carry; i++) {
      if (i == ret.dat.size()) ret.dat.emplace_back(0);
      ret.dat[i] += carry + (i < dat.size() ? dat[i] : 0);
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
    for (long long i = 0, carry = 0, ed = dat.size(); i < ed || carry; i++) {
      if (i == ed) dat.emplace_back(0);
      long long cur = r * dat[i] + carry;
      carry = cur / base, dat[i] = cur % base;
    }
    return shrink(), *this;
  }
  BigInt &operator/=(long long r) {
    if (r < 0) neg = !neg, r = -r;
    for (__int128_t i = dat.size() - 1, cur, rem = 0; i >= 0; i--)
      cur = dat[i] + (rem * base), dat[i] = cur / r, rem = cur % r;
    return shrink(), *this;
  }
  long long operator%(long long r) const {
    long long ret = 0;
    for (int i = dat.size(); i;) ret = (dat[--i] + (ret * base)) % r;
    return ret;
  }
  BigInt operator*(long long r) const { return BigInt(*this) *= r; }
  BigInt operator/(long long r) const { return BigInt(*this) /= r; }
  BigInt operator*(const BigInt &r) const {
    if (is_zero() || r.is_zero()) return 0;
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
      cur = carry + (i < sz ? h[i] : 0), carry = cur / base,
      ret.dat.emplace_back(cur % base);
    return ret.neg = neg ^ r.neg, ret.shrink(), ret;
  }
  BigInt &operator*=(const BigInt &r) { return *this = *this * r; }
  BigInt operator/(const BigInt &r) const {
    if (r.dat.size() == 1 && r.dat.back() == 1) return r.neg ? -*this : *this;
    if (this->abs() < r.abs()) return 0;
    int pb = dat.size(), qb = r.dat.size(), prec = std::max(pb - qb, 1),
        lim = std::min(prec, 3), rlim = std::min(qb, 6),
        nlim = std::min(lim * 2 + 1, prec), nrlim = std::min(rlim * 2 + 1, qb);
    BigInt x, prev, rr = r.base_shift_r(qb - rlim), c;
    x.dat.resize(lim + 1, 0), x.dat.back() = 1;
    c.dat.resize(rlim + lim + 1, 0), c.dat.back() = 2;
    while (x != prev) prev = x, x *= c - rr * x, x = x.base_shift_r(lim + rlim);
    if (lim != prec) {
      for (prev.neg = true; x != prev; lim = nlim, rlim = nrlim) {
        nlim = std::min(lim * 2 + 1, prec), nrlim = std::min(rlim * 2 + 1, qb);
        prev = x, x *= c - rr * x, x = x.base_shift_r(2 * lim - nlim + rlim);
        if (nrlim != rlim) rr = r.base_shift_r(qb - nrlim);
        if (nrlim != rlim || nlim != lim)
          c.dat.back() = 0, c.dat.resize(nrlim + nlim + 1, 0), c.dat.back() = 2;
      }
    }
    x *= this->abs(), x = x.base_shift_r(pb + (pb == qb));
    if (this->abs() >= (x + 1) * r.abs()) x += 1;
    return x.neg = neg ^ r.neg, x.shrink(), x;
  }
  BigInt &operator/=(const BigInt &r) { return *this = *this / r; }
  BigInt &operator%=(const BigInt &r) { return *this -= (*this / r) * r; }
  BigInt operator%(const BigInt &r) const { return BigInt(*this) %= r; }
  friend std::istream &operator>>(std::istream &is, BigInt &v) {
    std::string s;
    return is >> s, v.read(s), is;
  }
  friend std::ostream &operator<<(std::ostream &os, const BigInt &v) {
    return os << v.to_str(), os;
  }
};
