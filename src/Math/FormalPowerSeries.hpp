#pragma once
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
/**
 * @title 形式的冪級数
 * @category 数学
 */
// verify用: https://loj.ac/problem/150

// BEGIN CUT HERE

template <class mint>
struct FormalPowerSeries : std::vector<mint> {
  using FPS = FormalPowerSeries<mint>;
  using std::vector<mint>::vector;
  using m64_1 = ModInt<34703335751681, 3>;
  using m64_2 = ModInt<35012573396993, 3>;

 private:
  static inline m64_1 a1[1 << 21], b1[1 << 21], c1[1 << 21];
  static inline m64_2 a2[1 << 21], b2[1 << 21], c2[1 << 21];
  static inline mint bf1[1 << 21], bf2[1 << 21];
  template <class mod_t>
  static inline void idft(int n, mod_t x[]) {
    static mod_t iW[1 << 20];
    static constexpr std::uint64_t mod = mod_t::modulo();
    static constexpr unsigned pr = mod_t::pr_rt();
    static_assert(pr != 0);
    static constexpr mod_t G(pr);
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
    static constexpr unsigned pr = mod_t::pr_rt();
    static_assert(pr != 0);
    static constexpr mod_t G(pr);
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
  static inline void crt(m64_1 f1[], m64_2 f2[], int b, int e, mint ret[]) {
    static constexpr m64_2 iv = m64_2(m64_1::modulo()).inverse();
    static constexpr mint mod1 = m64_1::modulo();
    for (int i = b; i < e; i++) {
      std::uint64_t r1 = f1[i].get(), r2 = f2[i].get();
      ret[i] = mint(r1)
               + mint((m64_2(r2 + m64_2::modulo() - r1) * iv).get()) * mod1;
    }
  }
  template <typename T,
            typename enable_if<is_integral<T>::value>::type * = nullptr>
  static inline void subst(m64_1 f1[], m64_2 f2[], int b, int e, T ret[]) {
    for (int i = b; i < e; i++) f1[i] = ret[i], f2[i] = ret[i];
  }
  template <typename T,
            typename enable_if<!is_integral<T>::value>::type * = nullptr>
  static inline void subst(m64_1 f1[], m64_2 f2[], int b, int e, T ret[]) {
    std::uint64_t tmp;
    for (int i = b; i < e; i++) tmp = ret[i].get(), f1[i] = tmp, f2[i] = tmp;
  }
  static inline mint get_inv(int i) {
    static mint INV[1 << 21];
    static int lim = 0;
    static constexpr std::uint64_t mod = mint::modulo();
    if (lim <= i) {
      if (lim == 0) INV[1] = 1, lim = 2;
      for (int j = lim; j <= i; j++) INV[j] = INV[mod % j] * (mod - mod / j);
      lim = i + 1;
    }
    return INV[i];
  }

 public:
  int deg() const {
    int n = int(this->size()) - 1;
    while (n >= 0 && (*this)[n] == mint(0)) n--;
    return n;
  }
  FPS &norm() { return this->resize(max(this->deg() + 1, 1)), *this; }
  std::uint64_t inline get_len(std::uint64_t n) const {
    return --n, n |= n >> 1, n |= n >> 2, n |= n >> 4, n |= n >> 8,
           n |= n >> 16, n |= n >> 32, ++n;
  }
  FPS mul(const FPS &y) const {
    if (deg() == -1 || y.deg() == -1) return {0};
    int n = this->size(), m = y.size(), sz = n + m - 1;
    FPS ret(sz, 0);
    if (std::min(n, m) <= 8) {
      for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) ret[i + j] += (*this)[i] * y[j];
    } else {
      subst(a1, a2, 0, n, this->data()), subst(b1, b2, 0, m, y.data());
      int len = get_len(sz);
      std::fill(a1 + n, a1 + len, 0), std::fill(b1 + m, b1 + len, 0);
      std::fill(a2 + n, a2 + len, 0), std::fill(b2 + m, b2 + len, 0);
      dft(len, a1), dft(len, b1), dft(len, a2), dft(len, b2);
      for (int i = 0; i < len; i++) a1[i] *= b1[i], a2[i] *= b2[i];
      idft(len, a1), idft(len, a2), crt(a1, a2, 0, sz, ret.data());
    }
    return ret;
  }
  FPS inv() const {
    assert(!this->empty() && (*this)[0] != mint(0));
    int n = this->size(), len = get_len(n);
    std::copy_n(this->begin(), n, bf1), std::fill(bf1 + n, bf1 + len, 0);
    FPS ret(len, 0);
    ret[0] = bf1[0].inverse();
    for (int i = 1; i < 32 && i < n; i++) {
      for (int j = 1; j <= i; j++) ret[i] += bf1[j] * ret[i - j];
      ret[i] *= -ret[0];
    }
    for (int i = 64; i <= len; i <<= 1) {
      subst(a1, a2, 0, i, bf1), subst(b1, b2, 0, i, ret.data());
      dft(i, a1), dft(i, b1), dft(i, a2), dft(i, b2);
      for (int j = i - 1; j >= 0; j--) a1[j] *= b1[j], a2[j] *= b2[j];
      idft(i, a1), idft(i, a2);
      crt(a1, a2, i >> 1, i, ret.data()), subst(a1, a2, i >> 1, i, ret.data());
      std::fill_n(a1, i >> 1, 0), std::fill_n(a2, i >> 1, 0), dft(i, a1),
          dft(i, a2);
      for (int j = i - 1; j >= 0; j--) a1[j] *= b1[j], a2[j] *= b2[j];
      idft(i, a1), idft(i, a2), crt(a1, a2, i >> 1, i, ret.data());
      for (int j = i >> 1; j < i; j++) ret[j] = -ret[j];
    }
    return ret.resize(n), ret;
  }
  inline FPS div_con(const FPS &g, const FPS &g0) const {
    if (this->size() == 1) return {(*this)[0] * g[0].inverse()};
    int n = this->size(), len = get_len(n), len2 = len >> 1,
        m = std::min<int>(n, g.size());
    FPS ret(n);
    std::copy_n(this->begin(), n, bf1), std::fill(bf1 + n, bf1 + len, 0);
    std::copy_n(g.begin(), m, bf2), std::fill(bf2 + m, bf2 + len, 0);
    subst(a1, a2, 0, len2, g0.data()), subst(b1, b2, 0, len2, bf1);
    std::fill(a1 + len2, a1 + len, 0), std::fill(a2 + len2, a2 + len, 0);
    std::fill(b1 + len2, b1 + len, 0), std::fill(b2 + len2, b2 + len, 0);
    dft(len, a1), dft(len, b1), dft(len, a2), dft(len, b2);
    for (int i = 0; i < len; i++) b1[i] *= a1[i], b2[i] *= a2[i];
    idft(len, b1), idft(len, b2), crt(b1, b2, 0, len >> 1, ret.data());
    subst(b1, b2, 0, len2, ret.data()), subst(c1, c2, 0, len, bf2);
    std::fill(b1 + len2, b1 + len, 0), std::fill(b2 + len2, b2 + len, 0);
    dft(len, c1), dft(len, b1), dft(len, c2), dft(len, b2);
    for (int i = 0; i < len; i++) c1[i] *= b1[i], c2[i] *= b2[i];
    idft(len, c1), idft(len, c2), crt(c1 + len2, c2 + len2, 0, len2, bf1);
    for (int i = len2; i < len; i++) bf1[i] -= bf1[i - len2];
    subst(c1, c2, len2, len, bf1);
    std::fill_n(c1, len2, 0), std::fill_n(c2, len2, 0), dft(len, c1),
        dft(len, c2);
    for (int i = len; i >= 0; i--) c1[i] *= a1[i], c2[i] *= a2[i];
    idft(len, c1), idft(len, c2), crt(c1, c2, len2, n, ret.data());
    return ret;
  }
  inline std::pair<FPS, FPS> quorem_rev_con(const FPS &yr,
                                            const FPS &g0r) const {
    if (this->size() < yr.size()) return std::make_pair(FPS{0}, *this);
    int sq = this->size() - yr.size() + 1, len = get_len(sq);
    FPS qr = FPS(this->begin(), this->begin() + sq).div_con(yr, g0r);
    if (yr.size() == 1) return std::make_pair(qr, FPS{0});
    len = get_len(max(qr.size(), yr.size()));
    int mask = len - 1;
    subst(a1, a2, 0, sq, qr.data()), subst(b1, b2, 0, yr.size(), yr.data());
    std::fill(a1 + sq, a1 + len, 0), std::fill(a2 + sq, a2 + len, 0);
    std::fill(b1 + yr.size(), b1 + len, 0),
        std::fill(b2 + yr.size(), b2 + len, 0);
    dft(len, a1), dft(len, a2), dft(len, b1), dft(len, b2);
    for (int i = len - 1; i >= 0; i--) a1[i] *= b1[i], a2[i] *= b2[i];
    idft(len, a1), idft(len, a2), crt(a1, a2, 0, len, bf1);
    for (int i = sq - 1; i >= 0; i--) bf1[i & mask] -= (*this)[i & mask];
    FPS rem(this->begin() + sq, this->end());
    for (int i = rem.size() - 1; i >= 0; i--) rem[i] -= bf1[(sq + i) & mask];
    return std::make_pair(qr, rem);
  }
  inline std::pair<FPS, FPS> quorem_rev_n(const FPS &yr) const {
    if (this->size() < yr.size()) return std::make_pair(FPS{0}, *this);
    int sq = this->size() - yr.size() + 1;
    std::copy_n(this->begin(), this->size(), bf1);
    FPS qr(sq, 0);
    mint iv = yr[0].inverse();
    for (int i = 0; i < sq; i++) {
      qr[i] = bf1[i] * iv;
      for (int j = 0; j < (int)yr.size(); j++) bf1[j + i] -= yr[j] * qr[i];
    }
    return std::make_pair(qr, FPS(bf1 + sq, bf1 + this->size()));
  }
  FPS div(const FPS &y) const {
    if (this->size() == 1) return {(*this)[0] * y[0].inverse()};
    int len2 = get_len(this->size()) / 2;
    FPS g(len2);
    for (int i = std::min<int>(y.size(), len2) - 1; i >= 0; i--) g[i] = y[i];
    return div_con(y, g.inv());
  }
  FPS quo(FPS y) const {
    FPS x(*this);
    x.norm(), y.norm();
    if (x.size() < y.size()) return FPS{0};
    if (x.size() == y.size()) return FPS{x.back() / y.back()};
    reverse(x.begin(), x.end()), reverse(y.begin(), y.end());
    FPS ret = y.size() < 1024 ? x.quorem_rev_n(y).first
                              : (x.resize(x.size() - y.size() + 1), x.div(y));
    reverse(ret.begin(), ret.end());
    return ret;
  }
  std::pair<FPS, FPS> quorem(FPS y) const {
    FPS x(*this);
    x.norm(), y.norm();
    if (x.size() < y.size()) return std::make_pair(FPS{0}, x);
    if (x.size() == y.size()) {
      mint tmp = x.back() / y.back();
      return std::make_pair(FPS{tmp}, x - y * tmp);
    }
    reverse(x.begin(), x.end()), reverse(y.begin(), y.end());
    FPS q, r;
    if (y.size() < 1024)
      tie(q, r) = x.quorem_rev_n(y);
    else {
      int len2 = get_len(x.size() - y.size() + 1) / 2;
      FPS gr(len2);
      for (int i = std::min<int>(y.size(), len2) - 1; i >= 0; i--) gr[i] = y[i];
      tie(q, r) = x.quorem_rev_con(y, gr.inv());
    }
    reverse(q.begin(), q.end()), reverse(r.begin(), r.end());
    return std::make_pair(q, r.norm());
  }
  FPS diff() const {
    FPS ret(max(0, int(this->size() - 1)));
    for (int i = this->size() - 1; i > 0; i--) ret[i - 1] = (*this)[i] * i;
    return ret;
  }
  FPS inte() const {
    int len = this->size() + 1;
    FPS ret(len);
    ret[0] = 0;
    for (int i = len - 1; i >= 1; i--) ret[i] = (*this)[i - 1] * get_inv(i);
    return ret;
  }
  FPS log() const {
    assert((*this)[0] == mint(1));
    return this->size() == 1 ? FPS{0} : this->diff().div(*this).inte();
  }
  FPS exp() const {
    assert((*this)[0] == mint(0));
    int n = this->size(), len = get_len(n);
    if (n == 1) return {1};
    static mint b[1 << 21], f[1 << 21];
    std::copy_n(this->data(), n, bf1), std::fill(bf1 + n, bf1 + len, 0);
    FPS ret(len, 0);
    std::fill_n(bf2, len, 0), std::fill_n(c1, len, 0), std::fill_n(c2, len, 0);
    ret[0] = 1, ret[1] = bf1[1], bf2[0] = 1, bf2[1] = -bf1[1];
    for (int i = 1; i != len; ++i) b[i - 1] = mint(i) * bf1[i];
    subst(c1, c2, 0, 2, ret.data()), dft(4, c1), dft(4, c2);
    std::uint64_t tmp;
    for (int i = 4, i2 = 2; i <= len; i <<= 1, i2 <<= 1) {
      for (int j = i >> 2; j < i2; j++) f[j - 1] = ret[j] * mint(j);
      subst(b1, b2, 0, i2 - 1, b), b1[i2 - 1] = 0, b2[i2 - 1] = 0;
      dft(i2, b1), dft(i2, b2);
      for (int j = i2 - 1; j >= 0; j--) b1[j] *= c1[j], b2[j] *= c2[j];
      idft(i2, b1), idft(i2, b2), crt(b1, b2, 0, i2, bf1);
      for (int j = i2 - 3; j >= 0; j--)
        tmp = (f[j] - bf1[j]).get(), b1[j + i2] = tmp, b2[j + i2] = tmp;
      tmp = (bf1[i2 - 2] - f[i2 - 2]).get(), b1[i2 - 2] = tmp, b2[i2 - 2] = tmp;
      tmp = (-bf1[i2 - 1]).get(), b1[i2 - 1] = tmp, b2[i2 - 1] = tmp;
      std::fill_n(b1, i2 - 2, 0), std::fill_n(b2, i2 - 2, 0);
      b1[i - 2] = b1[i - 1] = 0, b2[i - 2] = b2[i - 1] = 0;
      subst(a1, a2, 0, i, bf2), dft(i, a1), dft(i, a2), dft(i, b1), dft(i, b2);
      for (int j = 0; j < i; j++) b1[j] *= a1[j], b2[j] *= a2[j];
      idft(i, b1), idft(i, b2), crt(b1, b2, i2 - 1, i - 1, bf2 + 1);
      for (int j = i - 1; j >= i2; j--) (bf2[j] *= get_inv(j)) -= bf1[j];
      subst(b1, b2, i2, i, bf2);
      std::fill_n(b1, i2, 0), std::fill_n(b2, i2, 0), dft(i, b1), dft(i, b2);
      for (int j = i - 1; j >= 0; j--) c1[j] *= b1[j], c2[j] *= b2[j];
      idft(i, c1), idft(i, c2), crt(c1, c2, i2, i, ret.data());
      for (int j = i2; j < i; j++) ret[j] = -ret[j];
      if (i != len) {
        subst(c1, c2, 0, i, ret.data()), dft(i << 1, c1), dft(i << 1, c2);
        for (int j = i - 1; j >= 0; j--)
          b1[j] = c1[j] * a1[j], b2[j] = c2[j] * a2[j];
        idft(i, b1), idft(i, b2), crt(b1, b2, i2, i, bf2);
        subst(b1, b2, i2, i, bf2);
        std::fill_n(b1, i2, 0), std::fill_n(b2, i2, 0), dft(i, b1), dft(i, b2);
        for (int j = i - 1; j >= 0; j--) b1[j] *= a1[j], b2[j] *= a2[j];
        idft(i, b1), idft(i, b2), crt(b1, b2, i2, i, bf2);
        for (int j = i2; j < i; j++) bf2[j] = -bf2[j];
      }
    }
    return ret.resize(n), ret;
  }
  FPS pow(std::uint64_t k) const {
    int n = this->size(), cnt = 0;
    while (cnt < n && (*this)[cnt] == mint(0)) cnt++;
    if (k * cnt >= (std::uint64_t)n) return FPS(n, 0);
    mint iv = (*this)[cnt].inverse();
    FPS pt = ((FPS(this->begin() + cnt, this->end()) * iv).log() * k).exp()
             * (*this)[cnt].pow(k),
        ret(n, 0);
    for (int i = k * cnt, j = 0; i < n; i++, j++) ret[i] = pt[j];
    return ret;
  }
  std::pair<FPS, FPS> cos_and_sin() const {
    static constexpr mint imag = mint(-1).sqrt();
    static constexpr mint iv2 = mint(mint::modulo() - (mint::modulo() - 1) / 2);
    FPS a = (*this * imag).exp(), b = (*this * (-imag)).exp();
    return std::make_pair((a + b) * iv2, (a - b) * iv2 / imag);
  }
  FPS sqrt() const {
    static constexpr mint iv2 = mint(mint::modulo() - (mint::modulo() - 1) / 2);
    int n = this->size(), cnt = 0;
    while (cnt < n && (*this)[cnt] == mint(0)) cnt++;
    if (cnt == n) return FPS(n, 0);
    if (cnt & 1) return FPS();  // no solution
    mint sqr = (*this)[cnt].sqrt();
    if (sqr * sqr != (*this)[cnt]) return FPS();  // no solution
    int len = get_len(n - cnt / 2);
    FPS ret({sqr});
    ret.resize(len);
    std::copy_n(this->begin() + cnt, n - cnt, bf1),
        std::fill(bf1 + n - cnt, bf1 + len, 0);
    std::fill_n(bf2, len, 0), bf2[0] = ret[0].inverse();
    std::uint64_t tmp;
    tmp = ret[0].get(), b1[0] = tmp, b2[0] = tmp, b1[1] = 0, b2[1] = 0;
    dft(2, b1), dft(2, b2);
    for (int i = 2, i2 = 1; i <= len; i <<= 1, i2 <<= 1) {
      for (int j = i2 - 1; j >= 0; j--)
        c1[j] = b1[j] * b1[j], c2[j] = b2[j] * b2[j];
      idft(i2, c1), idft(i2, c2), crt(c1, c2, 0, i2, ret.data() + i2);
      for (int j = i - 2; j >= i2; j--)
        tmp = (ret[j] - bf1[j - i2] - bf1[j]).get(), c1[j] = tmp, c2[j] = tmp;
      tmp = (ret[i - 1] - bf1[i2 - 1]).get(), c1[i2 - 1] = tmp,
      c2[i2 - 1] = tmp;
      tmp = (-bf1[i - 1]).get(), c1[i - 1] = tmp, c2[i - 1] = tmp;
      std::fill_n(c1, i2 - 1, 0), std::fill_n(c2, i2 - 1, 0),
          subst(a1, a2, 0, i, bf2);
      dft(i, a1), dft(i, a2), dft(i, c1), dft(i, c2);
      for (int j = i - 1; j >= 0; j--) c1[j] *= a1[j], c2[j] *= a2[j];
      idft(i, c1), idft(i, c2), crt(c1, c2, i2, i, bf2);
      for (int j = i2; j < i; j++) ret[j] = -bf2[j] * iv2;
      if (i != len) {
        subst(b1, b2, 0, i, ret.data()), dft(i, b1), dft(i, b2);
        for (int j = i - 1; j >= 0; j--)
          c1[j] = b1[j] * a1[j], c2[j] = b2[j] * a2[j];
        idft(i, c1), idft(i, c2), crt(c1, c2, i2, i, bf2);
        subst(c1, c2, i2, i, bf2);
        std::fill_n(c1, i2, 0), std::fill_n(c2, i2, 0), dft(i, c1), dft(i, c2);
        for (int j = i - 1; j >= 0; j--) c1[j] *= a1[j], c2[j] *= a2[j];
        idft(i, c1), idft(i, c2), crt(c1, c2, i2, i, bf2);
        for (int j = i2; j < i; j++) bf2[j] = -bf2[j];
      }
    }
    for (int i = n - cnt / 2 - 1; i >= 0; i--) ret[i + cnt / 2] = ret[i];
    std::fill_n(ret.begin(), cnt / 2, 0);
    return ret;
  }
  FPS shift(mint c) const {
    int n = this->size();
    FPS ret(n), p(n);
    mint f = 1, cpw = 1;
    for (int i = 0; i < n; f *= ++i) ret[n - i - 1] = (*this)[i] * f;
    f = mint(n) / f;
    for (int i = n; i > 0; i--) p[i - 1] = i == n ? f : p[i] * i;
    for (int i = 0; i < n; i++, cpw *= c) p[i] *= cpw;
    p *= ret;
    std::reverse_copy(p.begin(), p.begin() + n, ret.begin());
    for (int i = n - 1; i >= 2; f *= i--) ret[i] *= f;
    return ret;
  }
  FPS comp(const FPS &g) const {
    int n = this->size(), k = std::sqrt(1. * n);
    if (k * k < n) k++;
    int d = (n - 1 + k) / k;
    std::vector<FPS> gpw(d + 1);
    gpw[0] = {1};
    for (int i = 1; i <= d; i++) {
      gpw[i] = gpw[i - 1] * g;
      if ((int)gpw[i].size() > n) gpw[i].resize(n);
    }
    FPS ret(n, 0), gd{1}, tmp;
    for (int i = 0; i < k; i++) {
      tmp = {(*this)[i * d]};
      for (int j = 1; j < d && i * d + j < n; j++)
        tmp += gpw[j] * (*this)[i * d + j];
      tmp *= gd;
      for (int j = std::min<int>(n, tmp.size()) - 1; j >= 0; j--)
        ret[j] += tmp[j];
      gd *= gpw[d];
      if ((int)gd.size() > n) gd.resize(n);
    }
    return ret;
  }
  mint eval(mint x) const {
    mint res, w = 1;
    for (auto &v : *this) res += w * v, w *= x;
    return res;
  }
  FPS operator-() {
    FPS ret(this->size());
    for (int i = ret.size() - 1; i >= 0; i--) ret[i] = -(*this)[i];
    return ret;
  }
  FPS &operator*=(const mint &v) {
    for (int i = this->size() - 1; i >= 0; i--) (*this)[i] *= v;
    return *this;
  }
  FPS &operator/=(const mint &v) {
    for (int i = this->size() - 1; i >= 0; i--) (*this)[i] /= v;
    return *this;
  }
  FPS &operator+=(const FPS &r) {
    if (this->size() < r.size()) this->resize(r.size(), 0);
    for (int i = r.size() - 1; i >= 0; i--) (*this)[i] += r[i];
    return this->norm();
  }
  FPS &operator-=(const FPS &r) {
    if (this->size() < r.size()) this->resize(r.size(), 0);
    for (int i = r.size() - 1; i >= 0; i--) (*this)[i] -= r[i];
    return this->norm();
  }
  FPS &operator*=(const FPS &r) { return *this = norm().mul(FPS(r).norm()); }
  FPS &operator/=(const FPS &r) { return *this = this->quo(r); }
  FPS &operator%=(const FPS &r) { return *this = this->quorem(r).second; }
  FPS operator*(const mint &v) const { return FPS(*this) *= v; }
  FPS operator/(const mint &v) const { return FPS(*this) /= v; }
  FPS operator+(const FPS &r) const { return FPS(*this) += r; }
  FPS operator-(const FPS &r) const { return FPS(*this) -= r; }
  FPS operator*(const FPS &r) const { return FPS(*this) *= r; }
  FPS operator/(const FPS &r) const { return this->quo(r); }
  FPS operator%(const FPS &r) const { return this->quorem(r).second; }
};
