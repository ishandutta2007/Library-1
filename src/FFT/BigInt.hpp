#pragma once
#include <bits/stdc++.h>
#include "src/FFT/NTT.hpp"

/**
 * @title 多倍長整数
 * @category FFT
 */

// BEGIN CUT HERE
class BigInt {
  static constexpr unsigned BASE = 10000000, D = 7;
  using mod_t = StaticModInt<0x3ffffffffa000001>;
  using Vec = std::vector<unsigned>;
  using NTT = NumberTheoreticTransform<mod_t>;
  bool neg;
  Vec dat;
  BigInt shift(int sz) const { return {neg, Vec(dat.begin() + sz, dat.end())}; }
  BigInt(bool n, const Vec &d) : neg(n), dat(d) {}

 public:
  BigInt() : neg(false), dat() {}
  BigInt(long long v) : neg(v < 0) {
    for (v = std::abs(v); v; v /= BASE) dat.push_back(v % BASE);
  }
  BigInt(const std::string &s) : neg(false) {
    int p = 0, x = 0;
    for (; p < (int)s.size() && (s[p] == '-' || s[p] == '+'); p++)
      if (s[p] == '-') neg = !neg;
    for (int i = s.size(), j; i > p; i -= D, dat.push_back(x), x = 0)
      for (j = std::max(p, i - int(D)); j < i;) x = x * 10 + s[j++] - '0';
    shrink();
  }
  inline void shrink() {
    while (!dat.empty() && !dat.back()) dat.pop_back();
    if (dat.empty()) neg = false;
  }
  std::string to_str() const {
    std::stringstream ss;
    if (neg) ss << '-';
    ss << (dat.empty() ? 0 : dat.back());
    for (long long i = dat.size() - 1; i-- > 0;)
      ss << std::setw(D) << std::setfill('0') << dat[i];
    std::string ret;
    return ss >> ret, ret;
  }
  bool is_zero() const { return dat.empty() || (dat.size() == 1 && !dat[0]); }
  bool operator<(const BigInt &r) const {
    if (neg != r.neg) return neg;
    if (dat.size() != r.dat.size()) return (dat.size() < r.dat.size()) ^ neg;
    for (int i = dat.size(); i--;)
      if (dat[i] != r.dat[i]) return (dat[i] < r.dat[i]) ^ neg;
    return false;
  }
  bool operator>(const BigInt &r) const { return r < *this; }
  bool operator<=(const BigInt &r) const { return !(r < *this); }
  bool operator>=(const BigInt &r) const { return !(*this < r); }
  bool operator==(const BigInt &r) const {
    return (neg == r.neg && dat == r.dat) || (is_zero() && r.is_zero());
  }
  bool operator!=(const BigInt &r) const { return !(*this == r); }
  BigInt abs() const { return BigInt(false, dat); }
  BigInt operator-() const { return BigInt(!neg, dat); }
  BigInt operator+(const BigInt &r) const {
    if (neg != r.neg) return *this - (-r);
    auto [ret, tmp] = dat.size() > r.dat.size() ? std::make_pair(*this, &r)
                                                : std::make_pair(r, this);
    int car = 0, i, n = ret.dat.size(), m = tmp->dat.size();
    for (i = 0; i < m; i++)
      ret.dat[i] -= BASE & -(car = ((ret.dat[i] += car + tmp->dat[i]) >= BASE));
    if (car) {
      while (i < n && ret.dat[i] == BASE - 1) ret.dat[i++] = 0;
      i < n ? ret.dat[i]++ : (ret.dat.push_back(1), 0);
    }
    return ret;
  }
  BigInt operator-(const BigInt &r) const {
    if (neg != r.neg) return *this + (-r);
    if (r.is_zero()) return *this;
    if (is_zero()) return -r;
    auto [ret, tmp] =
        abs() > r.abs() ? std::make_pair(*this, &r) : std::make_pair(r, this);
    int car = 0, i, n = ret.dat.size(), m = tmp->dat.size();
    for (i = 0; i < m; i++)
      ret.dat[i] += BASE & -(car = ((ret.dat[i] -= car + tmp->dat[i]) >> 31));
    while (car && i < n && !ret.dat[i]) ret.dat[i++] = BASE - 1;
    return ret.neg ^= (tmp == this), ret.dat[i] -= car, ret.shrink(), ret;
  }
  long long operator%(long long r) const {
    long long ret = 0;
    for (int i = dat.size(); i--;) ret = (ret * BASE + dat[i]) % r;
    return ret;
  }
  BigInt operator*(const BigInt &r) const {
    if (is_zero() || r.is_zero()) return 0;
    const int n = dat.size(), m = r.dat.size(), sz = n + m - 1;
    static mod_t f[1 << 20], g[1 << 20];
    static long long h[1 << 20];
    if (int i = n, j; std::min(n, m) >= 74) {
      for (i = n; i--;) f[i] = dat[i];
      for (i = m; i--;) g[i] = r.dat[i];
      const int l = get_len(std::max(n, m)), bl = bsf(l) + 6;
      const int len = sz - l < bl * bl * 7 - 74 ? l : get_len(sz);
      std::fill_n(f + n, len - n, 0), NTT::dft(len, f);
      std::fill_n(g + m, len - m, 0), NTT::dft(len, g);
      for (i = len; i--;) f[i] *= g[i];
      for (NTT::idft(len, f), i = std::min(sz, len); i--;) h[i] = f[i].val();
      for (i = len, j; i < sz; h[i - len] -= h[i], i++)
        for (h[i] = 0, j = i - m + 1; j < n; j++) h[i] += dat[j] * r.dat[i - j];
    } else
      for (std::fill_n(h, sz, 0); i--;)
        for (int j = m; j--;) h[i + j] += (long long)dat[i] * r.dat[j];
    BigInt ret(neg ^ r.neg, Vec(sz));
    long long car = 0;
    for (int i = 0; i < sz; i++, car /= BASE) ret.dat[i] = (car += h[i]) % BASE;
    for (; car; car /= BASE) ret.dat.emplace_back(car % BASE);
    return ret;
  }
  BigInt operator/(const BigInt &r) const {
    assert(!r.is_zero());
    if (r.dat.size() == 1 && r.dat.back() == 1) return r.neg ? -*this : *this;
    BigInt a = this->abs(), b = r.abs();
    if (a < b) return 0;
    const int pb = dat.size(), qb = r.dat.size(), prec = std::max(pb - qb, 1);
    int l = std::min(prec, 3), ql = std::min(qb, 6), nl, nql;
    BigInt x(0, Vec(l + 1)), p, rr = b.shift(qb - ql), c(0, Vec(l + ql + 1));
    x.dat.back() = 1, c.dat.back() = 2;
    while (x != p) p.dat.swap(x.dat), x = (p * (c - rr * p)).shift(l + ql);
    if (l != prec)
      for (p.neg = true; x != p; l = nl, ql = nql) {
        nl = std::min(l * 2 + 1, prec), nql = std::min(ql * 2 + 1, qb);
        p.dat.swap(x.dat), x = (p * (c - rr * p)).shift(2 * l - nl + ql);
        if (p.neg = false; nql != ql) rr = b.shift(qb - nql);
        c.dat.back() = 0, c.dat.resize(nql + nl + 1), c.dat.back() = 2;
      }
    if (x = (x * a).shift(pb + (pb == qb)); a >= (x + 1) * b) x += 1;
    return x.neg = neg ^ r.neg, x;
  }
  BigInt operator%(const BigInt &r) const { return *this - (*this / r) * r; }
  BigInt &operator+=(const BigInt &r) { return *this = *this + r; }
  BigInt &operator-=(const BigInt &r) { return *this = *this - r; }
  BigInt &operator*=(const BigInt &r) { return *this = *this * r; }
  BigInt &operator/=(const BigInt &r) { return *this = *this / r; }
  BigInt &operator%=(const BigInt &r) { return *this = *this % r; }
  friend std::istream &operator>>(std::istream &is, BigInt &v) {
    std::string s;
    return is >> s, v = BigInt(s), is;
  }
  friend std::ostream &operator<<(std::ostream &os, const BigInt &v) {
    return os << v.to_str(), os;
  }
};