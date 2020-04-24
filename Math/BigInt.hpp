/**
 * @title 多倍長整数
 * @category 数学
 * @brief nttの配列のサイズに注意(REの原因になりがち)
 */

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
    int h = nh;
    while (!((r ^= h) & h)) h >>= 1;
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
      A[i] = a + b, A[i + 1] = a - b;
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
          A[i + m4 * 0] = t02 + t13, A[i + m4 * 2] = t02 - t13;
          t02 = a0 - a2, t13 = (a1 - a3) * imag;
          A[i + m4 * 1] = t02 + t13, A[i + m4 * 3] = t02 - t13;
        }
        w *= dw, w2 = w * w, w3 = w2 * w;
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

struct BigInt {
  using ll = long long;
  constexpr static ll base = 1000000000, base_digits = 9;

 private:
  bool minus;
  vector<ll> dat;

 public:
  BigInt() : minus(false) {}
  BigInt(ll v) { *this = v; }
  BigInt(const string &s) { read(s); }

 public:
  void shrink() {
    while (dat.size() && !dat.back()) dat.pop_back();
    if (dat.empty()) minus = false;
  }
  void read(const string &s) {
    minus = false;
    dat.clear();
    ll pos = 0;
    while (pos < (ll)s.size() && (s[pos] == '-' || s[pos] == '+')) {
      if (s[pos] == '-') minus = ~minus;
      ++pos;
    }
    for (ll i = s.size() - 1; i >= pos; i -= base_digits) {
      ll x = 0;
      for (ll j = max(pos, i - base_digits + 1); j <= i; j++)
        x = x * 10 + s[j] - '0';
      dat.push_back(x);
    }
    shrink();
  }
  BigInt &operator=(ll v) {
    minus = false;
    dat.clear();
    if (v < 0) minus = true, v = -v;
    for (; v > 0; v = v / base) dat.push_back(v % base);
    return *this;
  }
  bool isZero() const { return dat.empty() || (dat.size() == 1 && !dat[0]); }

 private:
  static vector<ll> mul_n(const vector<ll> &f, const vector<ll> &g) {
    vector<ll> ret(f.size() + g.size() - 1, 0);
    for (int i = 0; i < f.size(); i++)
      for (int j = 0; j < g.size(); j++) ret[i + j] += f[i] * g[j];
    return ret;
  }
  static void mul2(const vector<ll> &f, const vector<ll> &g,
                   bool cyclic = false) {
    using namespace ntt;
    for (int i = 0; i < (int)f.size(); i++) f1[i] = f[i], f2[i] = f[i];
    if (&f == &g) {
      convolute(f1, f.size(), f1, f.size(), cyclic);
      convolute(f2, f.size(), f2, f.size(), cyclic);
    } else {
      for (int i = 0; i < (int)g.size(); i++) g1[i] = g[i], g2[i] = g[i];
      convolute(f1, f.size(), g1, g.size(), cyclic);
      convolute(f2, f.size(), g2, g.size(), cyclic);
    }
  }
  static vector<ll> mul_crt(int beg, int end) {
    using namespace ntt;
    auto inv = m64_2(m64_1::modulo()).inverse();
    uint64_t mod1 = m64_1::modulo();
    vector<ll> ret(end - beg);
    for (int i = 0; i < (int)ret.size(); i++) {
      uint64_t r1 = f1[i + beg].get(), r2 = f2[i + beg].get();
      ret[i] = r1 + (m64_2(r2 + m64_2::modulo() - r1) * inv).get() * mod1;
    }
    return ret;
  }
  static vector<ll> convert_base(const vector<ll> &a, ll old_digits,
                                 ll new_digits) {
    vector<ll> p(max(old_digits, new_digits) + 1);
    p[0] = 1;
    for (ll i = 1; i < (ll)p.size(); i++) p[i] = p[i - 1] * 10;
    vector<ll> res;
    ll cur = 0;
    ll cur_digits = 0;
    for (ll i = 0; i < (ll)a.size(); i++) {
      cur += a[i] * p[cur_digits];
      cur_digits += old_digits;
      while (cur_digits >= new_digits) {
        res.push_back(signed(cur % p[new_digits]));
        cur /= p[new_digits];
        cur_digits -= new_digits;
      }
    }
    res.push_back((signed)cur);
    while (!res.empty() && !res.back()) res.pop_back();
    return res;
  }
  BigInt mul(const BigInt &v) const {
    if (this->isZero() || v.isZero()) return BigInt();
    constexpr static ll nbase = 10000, nbase_digits = 4;
    vector<ll> f = convert_base(this->dat, base_digits, nbase_digits);
    vector<ll> g = convert_base(v.dat, base_digits, nbase_digits);

    while (f.size() < g.size()) f.push_back(0);
    while (g.size() < f.size()) g.push_back(0);
    while (f.size() & (f.size() - 1)) f.push_back(0), g.push_back(0);
    vector<ll> h;
    if (f.size() + g.size() < 750) {
      h = mul_n(f, g);
    } else {
      mul2(f, g, false);
      h = mul_crt(0, int(f.size() + g.size() - 1));
    }
    BigInt res;
    res.minus = minus ^ v.minus;
    for (ll i = 0, carry = 0; i < (ll)h.size(); i++) {
      ll cur = h[i] + carry;
      res.dat.push_back((ll)(cur % nbase));
      carry = (ll)(cur / nbase);
      if (i + 1 == (int)h.size() && carry > 0) h.push_back(0);
    }
    res.dat = convert_base(res.dat, nbase_digits, base_digits);
    res.shrink();
    return res;
  }
  static pair<BigInt, BigInt> divmod(const BigInt &a1, const BigInt &b1) {
    ll norm = base / (b1.dat.back() + 1);
    BigInt a = a1.abs() * norm;
    BigInt b = b1.abs() * norm;
    BigInt q, r;
    q.dat.resize(a.dat.size());
    for (ll i = a.dat.size() - 1; i >= 0; i--) {
      r *= base;
      r += a.dat[i];
      ll s1 = r.dat.size() <= b.dat.size() ? 0 : r.dat[b.dat.size()];
      ll s2 = r.dat.size() <= b.dat.size() - 1 ? 0 : r.dat[b.dat.size() - 1];
      ll d = ((ll)base * s1 + s2) / b.dat.back();
      r -= b * d;
      while (r < 0) r += b, --d;
      q.dat[i] = d;
    }
    q.minus = a1.minus ^ b1.minus;
    r.minus = a1.minus;
    q.shrink(), r.shrink();
    return make_pair(q, r / norm);
  }

 public:
  bool operator<(const BigInt &v) const {
    if (minus != v.minus) return minus;
    if (dat.size() != v.dat.size()) return (dat.size() < v.dat.size()) ^ minus;
    for (ll i = dat.size() - 1; i >= 0; i--)
      if (dat[i] != v.dat[i]) return (dat[i] < v.dat[i]) ^ minus;
    return false;
  }
  bool operator>(const BigInt &v) const { return v < *this; }
  bool operator<=(const BigInt &v) const { return !(v < *this); }
  bool operator>=(const BigInt &v) const { return !(*this < v); }
  bool operator==(const BigInt &v) const {
    return minus == v.minus && dat == v.dat;
  }
  bool operator!=(const BigInt &v) const { return !(*this == v); }

 public:
  friend istream &operator>>(istream &stream, BigInt &v) {
    string s;
    stream >> s;
    v.read(s);
    return stream;
  }
  friend ostream &operator<<(ostream &stream, const BigInt &v) {
    if (v.minus) stream << '-';
    stream << (v.dat.empty() ? 0 : v.dat.back());
    for (ll i = (ll)v.dat.size() - 2; i >= 0; --i)
      stream << setw(base_digits) << setfill('0') << v.dat[i];
    return stream;
  }
  BigInt abs() const {
    BigInt res = *this;
    res.minus = false;
    return res;
  }
  BigInt operator-() const {
    BigInt res = *this;
    res.minus = ~res.minus;
    return res;
  }
  BigInt &operator*=(ll v) {
    if (v < 0) minus = ~minus, v = -v;
    for (ll i = 0, carry = 0; i < (ll)dat.size() || carry; ++i) {
      if (i == (ll)dat.size()) dat.push_back(0);
      ll cur = dat[i] * (ll)v + carry;
      carry = (ll)(cur / base);
      dat[i] = (ll)(cur % base);
    }
    shrink();
    return *this;
  }
  BigInt operator*(ll v) const { return BigInt(*this) *= v; }
  BigInt &operator/=(ll v) {
    if (v < 0) minus = ~minus, v = -v;
    for (ll i = (ll)dat.size() - 1, rem = 0; i >= 0; --i) {
      ll cur = dat[i] + rem * (ll)base;
      dat[i] = (ll)(cur / v);
      rem = (ll)(cur % v);
    }
    shrink();
    return *this;
  }
  BigInt operator/(ll v) const { return BigInt(*this) /= v; }
  ll operator%(ll v) const {
    assert(v > 0 && !minus);
    ll ret = 0;
    for (ll i = dat.size() - 1; i >= 0; --i)
      ret = (dat[i] + ret * (ll)base) % v;
    return ret;
  }
  BigInt operator+(const BigInt &v) const {
    if (minus != v.minus) return *this - (-v);
    BigInt res = v;
    for (ll i = 0, carry = 0; i < (ll)max(dat.size(), v.dat.size()) || carry;
         ++i) {
      if (i == (ll)res.dat.size()) res.dat.push_back(0);
      res.dat[i] += carry + (i < (ll)dat.size() ? dat[i] : 0);
      carry = res.dat[i] >= base;
      if (carry) res.dat[i] -= base;
    }
    return res;
  }
  BigInt operator-(const BigInt &v) const {
    if (minus != v.minus) return *this + (-v);
    if (abs() < v.abs()) return -(v - *this);
    BigInt res = *this;
    for (ll i = 0, carry = 0; i < (ll)v.dat.size() || carry; ++i) {
      res.dat[i] -= carry + (i < (ll)v.dat.size() ? v.dat[i] : 0);
      carry = res.dat[i] < 0;
      if (carry) res.dat[i] += base;
    }
    res.shrink();
    return res;
  }
  BigInt operator*(const BigInt &v) const { return this->mul(v); }
  BigInt operator/(const BigInt &v) const { return divmod(*this, v).first; }
  BigInt operator%(const BigInt &v) const { return divmod(*this, v).second; }

  BigInt &operator+=(const BigInt &v) { return *this = *this + v; }
  BigInt &operator-=(const BigInt &v) { return *this = *this - v; }
  BigInt &operator*=(const BigInt &v) { return *this = *this * v; }
  BigInt &operator/=(const BigInt &v) { return *this = *this / v; }
  BigInt &operator%=(const BigInt &v) { return *this = *this % v; }
};
