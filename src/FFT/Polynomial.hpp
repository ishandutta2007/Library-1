#pragma once
#include <bits/stdc++.h>
#include "src/FFT/fps_div.hpp"
#include "src/FFT/convolve.hpp"

/**
 * @title 多項式
 * @category FFT
 */

// BEGIN CUT HERE
template <class mod_t, std::size_t _Nm = 1 << 22>
class Polynomial : public std::vector<mod_t> {
  using Poly = Polynomial;
  struct Inde;
  struct XP_plus_C {  // x^p+c
    Inde x;
    mod_t c;
    XP_plus_C(const Inde &x_) : x(x_), c(Z) {}
    XP_plus_C(int p_, mod_t c_) : x(p_), c(c_) {}
  };
  struct Inde {  // indeterminate
    int p_;
    Inde(int p) : p_(p) {}
    Inde() : Inde(1) {}
    Inde operator^(int p) const { return Inde(p_ * p); }
    Inde operator*(const Inde &rhs) const { return Inde(p_ + rhs.p_); }
    int pow() const { return p_; }
    XP_plus_C operator+(mod_t c) { return XP_plus_C(p_, c); }
    XP_plus_C operator-(mod_t c) { return XP_plus_C(p_, -c); }
  };
  using GNA1 = GlobalNTTArray<mod_t, _Nm, 1>;
  using GNA2 = GlobalNTTArray<mod_t, _Nm, 2>;
  using GA = GlobalArray<mod_t, _Nm, 0>;
  using GAp = GlobalArray<mod_t, _Nm, 1>;
  using GAq = GlobalArray<mod_t, _Nm, 2>;
  using GA3 = GlobalArray<mod_t, _Nm, 3>;
  static inline const mod_t Z = 0;
  static constexpr int A = is_nttfriend<mod_t, _Nm>()      ? 8
                           : is_nttarraydouble<mod_t, _Nm> ? 17
                                                           : 20;
  static constexpr int B = is_nttfriend<mod_t, _Nm>()      ? 42
                           : is_nttarraydouble<mod_t, _Nm> ? 110
                                                           : 138;
  std::pair<Poly, Poly> quorem_na(const Poly &q) const {
    int n = deg(), m = q.deg(), qsz = n - m + 1, i = qsz, j;
    std::copy_n(this->begin(), n + 1, GAp::bf);
    std::copy_n(q.begin(), m + 1, GAq::bf);
    for (mod_t *bf = GAp::bf + n - m, iv = mod_t(1) / GAq::bf[m]; i--; bf--)
      for (GA::bf[i] = bf[j = m] * iv; j--;) bf[j] -= GAq::bf[j] * GA::bf[i];
    Poly rem(GAp::bf, GAp::bf + m);
    return {Poly(GA::bf, GA::bf + qsz), rem.shrink()};
  }
  Poly quo(const Poly &q) const {
    const int n = deg() + 1, m = q.deg() + 1, qsz = n - m + 1,
              nb = this->size() - n, mb = q.size() - m;
    auto ret = div<mod_t, _Nm>(
        Poly(this->rbegin() + nb, this->rbegin() + nb + qsz),
        Poly(q.rbegin() + mb, q.rbegin() + mb + std::min(qsz, m)));
    return std::reverse(ret.begin(), ret.end()), ret;
  }
  std::pair<Poly, Poly> quorem_ntt(const Poly &q) const {
    const int n = deg(), m = q.deg(), qsz = n - m + 1;
    auto qu = quo(q);
    std::copy(qu.begin(), qu.end(), GA::bf);
    std::copy_n(this->begin(), n + 1, GAp::bf);
    std::copy_n(q.begin(), m + 1, GAq::bf);
    const int len = get_len(m), mask = len - 1;
    if (len > qsz) std::fill_n(GA::bf + qsz, len - qsz, Z);
    if (len > m + 1) std::fill_n(GAq::bf + m + 1, len - m - 1, Z);
    for (int i = qsz; i-- > len;) GA::bf[i & mask] += GA::bf[i];
    for (int i = n; i >= len; i--) GAp::bf[i & mask] += GAp::bf[i];
    if (GNA1::bf.set(GA::bf, 0, len); m == len) GAq::bf[0] += GAq::bf[m];
    GNA2::bf.set(GAq::bf, 0, len), GNA1::bf.dft(0, len), GNA2::bf.dft(0, len);
    GNA1::bf.mul(GNA2::bf, 0, len), GNA1::bf.idft(0, len);
    GNA1::bf.get(GAq::bf, 0, m);
    for (int i = m; i--;) GAp::bf[i] -= GAq::bf[i];
    Poly rem(GAp::bf, GAp::bf + m);
    return std::make_pair(qu, rem.shrink());
  }

 public:
  using std::vector<mod_t>::vector;
  Polynomial(mod_t a) : Polynomial(1, a) {}
  Polynomial(const std::vector<mod_t> &p) : Polynomial(p.begin(), p.end()) {}
  Polynomial(const XP_plus_C &xpc) : Polynomial(xpc.x.pow() + 1) {
    (*this)[xpc.x.pow()] = 1, (*this)[0] = xpc.c;
  }
  static Inde x() { return Inde(); }
  inline int deg() const {
    for (int n = this->size() - 1;; n--)
      if (n < 0 || (*this)[n] != Z) return n;
  }
  inline Poly &shrink() { return this->resize(std::max(deg() + 1, 1)), *this; }
#define ASSIGN(op)                                \
  Poly &operator op##=(const Poly &r) {           \
    const std::size_t n = r.deg() + 1;            \
    if (this->size() < n) this->resize(n);        \
    for (int i = n; i--;) (*this)[i] op## = r[i]; \
    return shrink();                              \
  }
  ASSIGN(+)
  ASSIGN(-)
#undef ASSIGN
  Poly &operator*=(const Poly &r) { return *this = *this * r, *this; }
  Poly &operator/=(const Poly &r) { return *this = *this / r, *this; }
  Poly &operator%=(const Poly &r) { return *this = *this % r, *this; }
  Poly operator-() const { return Poly() -= *this; }
  Poly operator+(const Poly &r) const { return Poly(*this) += r; }
  Poly operator-(const Poly &r) const { return Poly(*this) -= r; }
  Poly operator*(const Poly &r) const { return convolve<mod_t, _Nm>(*this, r); }
  Poly operator/(const Poly &r) const {
    const int m = r.deg(), qsz = deg() - m + 1, ln = bsf(get_len(qsz));
    assert(m >= 0);
    if (qsz <= 0) return Poly{Z};
    return m + 3 < A * ln + B || qsz <= 64 ? quorem_na(r).first : quo(r);
  }
  std::pair<Poly, Poly> quorem(const Poly &r) const {
    const int n = deg(), m = r.deg(), qsz = n - m + 1, ln = bsf(get_len(qsz));
    assert(m >= 0);
    if (qsz <= 0) return {Poly{Z}, Poly(this->begin(), this->begin() + n + 1)};
    return m < A * ln + B || qsz <= 64 ? quorem_na(r) : quorem_ntt(r);
  }
  Poly operator%(const Poly &r) const { return quorem(r).second; }
  Poly &operator+=(const mod_t r) { return *this[0] += r, *this; }
  Poly &operator-=(const mod_t r) { return *this[0] -= r, *this; }
  Poly &operator*=(const mod_t r) {
    for (mod_t &c : *this) c *= r;
    return shrink();
  }
  Poly &operator/=(const mod_t r) {
    for (mod_t &c : *this) c /= r;
    return shrink();
  }
  Poly operator+(const mod_t r) { return Poly(*this) += r; }
  Poly operator-(const mod_t r) { return Poly(*this) -= r; }
  Poly operator*(const mod_t r) { return Poly(*this) *= r; }
  Poly operator/(const mod_t r) { return Poly(*this) /= r; }
  friend Poly operator+(const mod_t l, Poly r) { return r += l; }
  friend Poly operator-(const mod_t l, Poly r) { return -(r -= l); }
  friend Poly operator*(const mod_t l, Poly r) { return r *= l; }
  mod_t operator()(mod_t c) const {  // eval f(c)
    if (c == Z) return (*this)[0];
    mod_t ret = 0;
    for (int i = deg() + 1; i--;) ret *= c, ret += (*this)[i];
    return ret;
  }
  Poly operator()(const XP_plus_C &xpc) const {  // f(x^p+c)
    return taylor_shift(xpc.c).scale(xpc.x.pow());
  }
  Poly operator()(const Poly &q) const {  // f(g) mod x^n
    const int n = this->deg() + 1, k = std::ceil(std::sqrt(n));
    std::vector<Poly> pw1(k + 1), pw2(k + 1);
    if (pw1[0] = {1}, pw1[1] = q; q.size() > n) pw1[1].resize(n);
    for (int i = 2; i <= k; ++i)
      if (pw1[i] = pw1[i - 1] * pw1[1]; pw1[i].size() > n) pw1[i].resize(n);
    pw2[0] = {1}, pw2[1] = pw1[k];
    for (int i = 2; i <= k; ++i)
      if (pw2[i] = pw2[i - 1] * pw2[1]; pw2[i].size() > n) pw2[i].resize(n);
    Poly ret(n, Z), f;
    for (int i = 0, j; i <= k; ++i) {
      for (f.assign(n, Z), j = std::min(k, std::max(0, n - k * i)); j--;) {
        mod_t coef = (*this)[k * i + j];
        for (int d = pw1[j].size(); d--;) f[d] += pw1[j][d] * coef;
      }
      for (f *= pw2[i], j = std::min<int>(n, f.size()); j--;) ret[j] += f[j];
    }
    return ret;
  }
  Poly &operator*=(const XP_plus_C &xpc) {
    Poly q;
    if (xpc.c != Z) q = *this * xpc.c;
    return this->insert(this->begin(), xpc.x.pow(), Z), *this += q;
  }
  Poly operator*(const XP_plus_C &xpc) const { return Poly(*this) *= xpc; }
  friend Poly operator*(const XP_plus_C &xpc, const Poly &p) { return p * xpc; }
  Poly scale(int k) const {
    const int n = deg();
    Poly ret(n * k + 1, Z);
    for (int i = 0; i <= n; i++) ret[i * k] += (*this)[i];
    return ret;
  }
  Poly taylor_shift(mod_t c) const {
    int n = deg(), i = 0;
    if (n < 1 || c == Z) return Poly(*this);
    mod_t cpw = 1, fact = 1;
    for (; i <= n; fact *= ++i) GAp::bf[n - i] = (*this)[i] * fact;
    for (fact = mod_t(1) / fact; i--;) GA3::bf[i] = (fact *= i + 1);
    for (; ++i <= n;) GAq::bf[i] = cpw * GA3::bf[i], cpw *= c;
    auto ret = Poly(GAp::bf, GAp::bf + n + 1) * Poly(GAq::bf, GAq::bf + n + 1);
    for (ret.resize(n + 1), std::reverse(ret.begin(), ret.end()); i--;)
      ret[i] *= GA3::bf[i];
    return ret;
  }
  friend std::ostream &operator<<(std::ostream &os, const Poly &p) {
    if (p.deg() == -1) return os << 0;
    for (int i = 0, e = p.deg(); i <= e; i++) {
      if (p[i] == Z) continue;
      if (i == 0 || p[i] != mod_t(1)) os << p[i];
      if (i >= 1) os << 'x';
      if (i > 9)
        os << "^(" << i << ')';
      else if (i > 1)
        os << '^' << i;
      if (i + 1 <= e) os << " + ";
    }
    return os;
  }
};