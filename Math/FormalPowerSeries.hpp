/**
 * @title 形式的冪級数(任意素数MOD)
 * @category 数学
 * @brief 998244353とかでなくても使える
 * @brief まずinitを呼んでmodを設定
 * @brief 配列のサイズに注意(REの原因になりがち)
 */
// verify用 https://loj.ac/problem/150

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

namespace ntt {
template <uint64_t mod, uint64_t prim_root>
class Mod64 {
 private:
  using u128 = __uint128_t;
  static constexpr uint64_t mul_inv(uint64_t n, int e = 6, uint64_t x = 1) {
    return e == 0 ? x : mul_inv(n, e - 1, x * (2 - x * n));
  }

 public:
  static constexpr uint64_t inv = mul_inv(mod, 6, 1);
  static constexpr uint64_t r2 = -u128(mod) % mod;
  static constexpr int level = __builtin_ctzll(mod - 1);
  static_assert(inv * mod == 1, "invalid 1/M modulo 2^64.");
  Mod64() {}
  Mod64(uint64_t n) : x(init(n)){};
  static uint64_t modulo() { return mod; }
  static uint64_t init(uint64_t w) { return reduce(u128(w) * r2); }
  static uint64_t reduce(const u128 w) {
    return uint64_t(w >> 64) + mod - ((u128(uint64_t(w) * inv) * mod) >> 64);
  }
  static Mod64 omega() { return Mod64(prim_root).pow((mod - 1) >> level); }
  Mod64 &operator+=(Mod64 rhs) {
    this->x += rhs.x;
    return *this;
  }
  Mod64 &operator-=(Mod64 rhs) {
    this->x += 2 * mod - rhs.x;
    return *this;
  }
  Mod64 &operator*=(Mod64 rhs) {
    this->x = reduce(u128(this->x) * rhs.x);
    return *this;
  }
  Mod64 operator+(Mod64 rhs) const { return Mod64(*this) += rhs; }
  Mod64 operator-(Mod64 rhs) const { return Mod64(*this) -= rhs; }
  Mod64 operator*(Mod64 rhs) const { return Mod64(*this) *= rhs; }
  uint64_t get() const { return reduce(this->x) % mod; }
  void set(uint64_t n) const { this->x = n; }
  Mod64 pow(uint64_t exp) const {
    Mod64 ret = Mod64(1);
    for (Mod64 base = *this; exp; exp >>= 1, base *= base)
      if (exp & 1) ret *= base;
    return ret;
  }
  Mod64 inverse() const { return pow(mod - 2); }
  uint64_t x;
};

template <typename mod_t>
void convolute(mod_t *A, int s1, mod_t *B, int s2, bool cyclic = false) {
  int s = (cyclic ? max(s1, s2) : s1 + s2 - 1);
  int size = 1;
  while (size < s) size <<= 1;
  mod_t roots[mod_t::level] = {mod_t::omega()};
  for (int i = 1; i < mod_t::level; i++) roots[i] = roots[i - 1] * roots[i - 1];
  fill(A + s1, A + size, 0);
  ntt_dit4(A, size, 1, roots);
  if (A == B && s1 == s2) {
    for (int i = 0; i < size; i++) A[i] *= A[i];
  } else {
    fill(B + s2, B + size, 0);
    ntt_dit4(B, size, 1, roots);
    for (int i = 0; i < size; i++) A[i] *= B[i];
  }
  ntt_dit4(A, size, -1, roots);
  mod_t inv = mod_t(size).inverse();
  for (int i = 0; i < (cyclic ? size : s); i++) A[i] *= inv;
}

template <typename mod_t>
void rev_permute(mod_t *A, int n) {
  int r = 0, nh = n >> 1;
  for (int i = 1; i < n; i++) {
    for (int h = nh; !((r ^= h) & h); h >>= 1)
      ;
    if (r > i) swap(A[i], A[r]);
  }
}

template <typename mod_t>
void ntt_dit4(mod_t *A, int n, int sign, mod_t *roots) {
  rev_permute(A, n);
  int logn = __builtin_ctz(n);
  if (logn & 1)
    for (int i = 0; i < n; i += 2) {
      mod_t a = A[i], b = A[i + 1];
      A[i] = a + b;
      A[i + 1] = a - b;
    }
  mod_t imag = roots[mod_t::level - 2];
  if (sign < 0) imag = imag.inverse();
  mod_t one = mod_t(1);
  for (int e = 2 + (logn & 1); e < logn + 1; e += 2) {
    const int m = 1 << e;
    const int m4 = m >> 2;
    mod_t dw = roots[mod_t::level - e];
    if (sign < 0) dw = dw.inverse();
    const int block_size = max(m, (1 << 15) / int(sizeof(A[0])));
    for (int k = 0; k < n; k += block_size) {
      mod_t w = one, w2 = one, w3 = one;
      for (int j = 0; j < m4; j++) {
        for (int i = k + j; i < k + block_size; i += m) {
          mod_t a0 = A[i + m4 * 0] * one, a2 = A[i + m4 * 1] * w2;
          mod_t a1 = A[i + m4 * 2] * w, a3 = A[i + m4 * 3] * w3;
          mod_t t02 = a0 + a2, t13 = a1 + a3;
          A[i + m4 * 0] = t02 + t13;
          A[i + m4 * 2] = t02 - t13;
          t02 = a0 - a2, t13 = (a1 - a3) * imag;
          A[i + m4 * 1] = t02 + t13;
          A[i + m4 * 3] = t02 - t13;
        }
        w *= dw;
        w2 = w * w;
        w3 = w2 * w;
      }
    }
  }
}

const int size = 1 << 22;
using m64_1 = ntt::Mod64<34703335751681, 3>;
using m64_2 = ntt::Mod64<35012573396993, 3>;
m64_1 f1[size], g1[size];
m64_2 f2[size], g2[size];

}  // namespace ntt

template <typename Modint>
struct FormalPowerSeries : vector<Modint> {
  using FPS = FormalPowerSeries;

 public:
  using vector<Modint>::vector;

 public:
  void shrink() {
    while (this->size() && this->back() == Modint(0)) this->pop_back();
  }
  FPS part(int beg, int end = -1) const {
    if (end < 0) end = beg, beg = 0;
    FPS ret(end - beg);
    for (int i = beg; i < min(end, int(this->size())); i++)
      ret[i - beg] = (*this)[i];
    return ret;
  }
  FPS operator>>(int size) const {
    if (this->size() <= size) return {};
    FPS ret(*this);
    ret.erase(ret.begin(), ret.begin() + size);
    return ret;
  }
  FPS operator<<(int size) const {
    FPS ret(*this);
    ret.insert(ret.begin(), size, Modint(0));
    return ret;
  }
  FPS rev() const {
    FPS ret(*this);
    reverse(ret.begin(), ret.end());
    return ret;
  }
  FPS operator-() {
    FPS ret(*this);
    for (int i = 0; i < (int)ret.size(); i++) ret[i] = -ret[i];
    return ret;
  }
  FPS &operator+=(const Modint &v) {
    (*this)[0] += v;
    return *this;
  }
  FPS &operator-=(const Modint &v) {
    (*this)[0] -= v;
    return *this;
  }
  FPS &operator*=(const Modint &v) {
    for (int k = 0; k < this->size(); k++) (*this)[k] *= v;
    return *this;
  }
  FPS &operator+=(const FPS &rhs) {
    if (this->size() < rhs.size()) this->resize(rhs.size());
    for (int i = 0; i < (int)rhs.size(); i++) (*this)[i] += rhs[i];
    return *this;
  }
  FPS &operator-=(const FPS &rhs) {
    if (this->size() < rhs.size()) this->resize(rhs.size());
    for (int i = 0; i < (int)rhs.size(); i++) (*this)[i] -= rhs[i];
    return *this;
  }
  FPS &operator*=(const FPS &rhs) { return *this = *this * rhs; }
  FPS &operator/=(const FPS &rhs) {
    if (this->size() < rhs.size()) return *this = FPS();
    FPS frev = this->rev();
    FPS rhsrev = rhs.rev();
    if (rhs.size() < 1150) return *this = frev.divrem_rev_n(rhsrev).first.rev();
    FPS inv = rhsrev.inverse(this->size() - rhs.size() + 1);
    return *this = frev.div_rev_pre(rhsrev, inv).rev();
  }
  FPS &operator%=(const FPS &rhs) {
    if (this->size() < rhs.size()) return *this;
    FPS frev = this->rev();
    FPS rhsrev = rhs.rev();
    if (rhs.size() < 1150)
      return *this = frev.divrem_rev_n(rhsrev).second.rev();
    FPS inv = rhsrev.inverse(frev.size() - rhs.size() + 1);
    return *this = frev.rem_rev_pre(rhsrev, inv).rev();
  }
  FPS operator+(const Modint &v) const { return FPS(*this) += v; }   // O(1)
  FPS operator-(const Modint &v) const { return FPS(*this) -= v; }   // O(1)
  FPS operator*(const Modint &v) const { return FPS(*this) *= v; }   // O(N)
  FPS operator+(const FPS &rhs) const { return FPS(*this) += rhs; }  // O(N)
  FPS operator-(const FPS &rhs) const { return FPS(*this) -= rhs; }  // O(N)
  FPS operator*(const FPS &rhs) const { return this->mul(rhs); }     // O(NlogN)
  FPS operator/(const FPS &rhs) const { return FPS(*this) /= rhs; }  // O(NlogN)
  FPS operator%(const FPS &rhs) const { return FPS(*this) %= rhs; }  // O(NlogN)

 public:
  static Modint mod_sqrt(Modint x) {
    if (x == 0 || Modint::modulo() == 2) return x;
    if (x.pow((Modint::modulo() - 1) >> 1) != 1) return 0;  // no solution
    Modint b(2);
    Modint w(b * b - x);
    while (w.pow((Modint::modulo() - 1) >> 1) == 1) {
      b += Modint(1);
      w = b * b - x;
    }
    auto mul = [&](pair<Modint, Modint> u, pair<Modint, Modint> v) {
      Modint a = (u.first * v.first + u.second * v.second * w);
      Modint b = (u.first * v.second + u.second * v.first);
      return make_pair(a, b);
    };
    unsigned exp = (Modint::modulo() + 1) >> 1;
    auto ret = make_pair(Modint(1), Modint(0));
    for (auto base = make_pair(b, 1); exp; exp >>= 1, base = mul(base, base)) {
      if (exp & 1) ret = mul(ret, base);
    }
    return ret.first.x * 2 < Modint::modulo() ? ret.first : -ret.first;
  }

 private:
  static void mul2(const FPS &f, const FPS &g, bool cyclic = false) {
    using namespace ntt;
    for (int i = 0; i < (int)f.size(); i++) f1[i] = f[i].x, f2[i] = f[i].x;
    if (&f == &g) {
      convolute(f1, f.size(), f1, f.size(), cyclic);
      convolute(f2, f.size(), f2, f.size(), cyclic);
    } else {
      for (int i = 0; i < (int)g.size(); i++) g1[i] = g[i].x, g2[i] = g[i].x;
      convolute(f1, f.size(), g1, g.size(), cyclic);
      convolute(f2, f.size(), g2, g.size(), cyclic);
    }
  }
  static FPS mul_crt(int beg, int end) {
    using namespace ntt;
    auto inv = m64_2(m64_1::modulo()).inverse();
    Modint mod1(m64_1::modulo());
    FPS ret(end - beg);
    for (int i = 0; i < (int)ret.size(); i++) {
      uint64_t r1 = f1[i + beg].get(), r2 = f2[i + beg].get();
      ret[i] = Modint(r1)
               + Modint((m64_2(r2 + m64_2::modulo() - r1) * inv).get()) * mod1;
    }
    return ret;
  }
  FPS mul_n(const FPS &g) const {
    if (this->size() == 0 || g.size() == 0) return FPS();
    FPS ret(this->size() + g.size() - 1, 0);
    for (int i = 0; i < this->size(); i++)
      for (int j = 0; j < g.size(); j++) ret[i + j] += (*this)[i] * g[j];
    return ret;
  }
  FPS mul(const FPS &g) const {
    if (this->size() == 0 || g.size() == 0) return FPS();
    if (this->size() + g.size() < 750) return mul_n(g);
    const FPS &f = *this;
    mul2(f, g, false);
    return mul_crt(0, int(f.size() + g.size() - 1));
  }
  FPS middle_product(const FPS &g) const {
    const FPS &f = *this;
    if (f.size() == 0 || g.size() == 0) return FPS();
    mul2(f, g, true);
    return mul_crt(f.size(), g.size());
  }
  FPS mul_cyclically(const FPS &g) const {
    const auto &f = *this;
    if (f.size() == 0 || g.size() == 0) return FPS();
    mul2(f, g, true);
    int s = max(f.size(), g.size()), size = 1;
    while (size < s) size <<= 1;
    return mul_crt(0, size);
  }
  static FPS sub_mul(const FPS &f, const FPS &q, const FPS &d) {
    int sq = q.size();
    FPS p = q.mul_cyclically(d);
    int mask = p.size() - 1;
    for (int i = 0; i < sq; i++) p[i & mask] -= f[i & mask];
    FPS r = f.part(sq, f.size());
    for (int i = 0; i < r.size(); i++) r[i] -= p[(sq + i) & mask];
    return r;
  }

 public:
  pair<FPS, FPS> divrem_rev_n(const FPS &brev) {
    FPS frev(*this);
    if (frev.size() < brev.size()) return make_pair(FPS(), frev);
    int sq = frev.size() - brev.size() + 1;
    FPS qrev(sq, 0);
    Modint inv = brev[0].inverse();
    for (int i = 0; i < qrev.size(); ++i) {
      qrev[i] = frev[i] * inv;
      for (int j = 0; j < brev.size(); ++j) frev[j + i] -= brev[j] * qrev[i];
    }
    return {qrev, frev.part(sq, frev.size())};
  }
  FPS div_rev_pre(const FPS &brev, const FPS &brevinv) const {
    if (this->size() < brev.size()) {
      return FPS();
    }
    int sq = this->size() - brev.size() + 1;
    assert(this->size() >= sq && brevinv.size() >= sq);
    FPS qrev = (this->part(sq) * brevinv.part(sq)).part(sq);
    return qrev;
  }
  FPS rem_rev_pre(const FPS &brev, const FPS &brevinv) const {
    if (this->size() < brev.size()) {
      return FPS(*this);
    }
    FPS rrev = sub_mul(*this, div_rev_pre(brev, brevinv), brev);
    return rrev;
  }

 private:
  FPS inverse(int deg = -1) const {
    if (deg < 0) deg = this->size();
    FPS ret(1, (*this)[0].inverse());
    for (int e = 1, ne; e < deg; e = ne) {
      ne = min(2 * e, deg);
      FPS h = ret.part(ne - e) * -ret.middle_product(this->part(ne));
      for (int i = e; i < ne; i++) ret.push_back(h[i - e]);
    }
    return ret;
  }
  FPS differentiation() const {
    FPS ret(max(0, int(this->size() - 1)));
    for (int i = 1; i < this->size(); i++) ret[i - 1] = (*this)[i] * Modint(i);
    return ret;
  }
  FPS integral() const {
    FPS ret(this->size() + 1);
    ret[0] = Modint(0);
    for (int i = 0; i < this->size(); i++)
      ret[i + 1] = (*this)[i] / Modint(i + 1);
    return ret;
  }
  FPS logarithm(int deg = -1) const {
    assert((*this)[0].x == 1);
    if (deg < 0) deg = this->size();
    return ((this->differentiation() * this->inverse(deg)).part(deg - 1))
        .integral();
  }
  FPS exponent(int deg = -1) const {
    assert((*this)[0].x == 0);
    if (deg < 0) deg = this->size();
    FPS ret({1, 1 < this->size() ? (*this)[1] : 0}), retinv(1, 1);
    FPS f = this->differentiation();
    FPS retdif = ret.differentiation();
    for (int e = 1, ne = 2, nne; ne < deg; e = ne, ne = nne) {
      nne = min(2 * ne, deg);
      FPS h = retinv.part(ne - e) * -retinv.middle_product(ret);
      for (int i = e; i < ne; i++) retinv.push_back(h[i - e]);
      FPS a = ret * f.part(nne) - retdif;
      FPS c = (retinv * a.part(nne)).integral();
      h = ret.middle_product(c.part(nne));
      for (int i = ne; i < nne; i++) {
        ret.push_back(h[i - ne]);
        retdif.push_back(Modint(i) * h[i - ne]);
      }
    }
    return ret;
  }
  FPS square_root(int deg = -1) const {
    if (deg < 0) deg = this->size();
    if ((*this)[0].x == 0) {
      for (int i = 1; i < this->size(); i++) {
        if ((*this)[i].x != 0) {
          if (i & 1) return FPS();  // no solution
          if (deg - i / 2 <= 0) break;
          auto ret = (*this >> i).square_root(deg - i / 2);
          if (!ret.size()) return FPS();  // no solution
          ret = ret << (i / 2);
          if (ret.size() < deg) ret.resize(deg, 0);
          return ret;
        }
      }
      return FPS(deg, 0);
    }
    Modint sqr = mod_sqrt((*this)[0]);
    if (sqr * sqr != (*this)[0]) return FPS();  // no solution
    FPS ret(1, sqr);
    Modint inv2 = Modint(2).inverse();
    for (int i = 1; i < deg; i <<= 1) {
      ret += *this->part(i << 1) * ret.inverse(i << 1);
      ret = ret.part(i << 1);
      ret *= inv2;
    }
    return ret;
  }
  FPS power(uint64_t k, int deg = -1) const {
    if (deg < 0) deg = this->size();
    for (int i = 0; i < this->size(); i++) {
      if ((*this)[i].x != 0) {
        if (i * k > deg) return FPS(deg, 0);
        Modint inv = (*this)[i].inverse();
        FPS ret = (((*this * inv) >> i).logarithm() * k).exponent()
                  * (*this)[i].pow(k);
        return (ret << (i * k)).part(deg);
      }
    }
    return *this;
  }

 public:
  FPS diff() const { return differentiation(); }                     // O(N)
  FPS inte() const { return integral(); }                            // O(N)
  FPS inv(int deg = -1) const { return inverse(deg); }               // O(NlogN)
  FPS log(int deg = -1) const { return logarithm(deg); }             // O(NlogN)
  FPS exp(int deg = -1) const { return exponent(deg); }              // O(NlogN)
  FPS sqrt(int deg = -1) const { return square_root(deg); }          // O(NlogN)
  FPS pow(uint64_t k, int deg = -1) const { return power(k, deg); }  // O(NlogN)
};