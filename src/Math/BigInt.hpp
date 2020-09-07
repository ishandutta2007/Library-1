/**
 * @title 多倍長整数
 * @category 数学
 * pythonの多倍長整数を使うべき
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

namespace ntt {
struct ModB {
  static constexpr uint64_t mask = (1ull << 32) - 1;
  static constexpr uint64_t mod = (((1ull << 32) - 1) << 32) + 1;
  static constexpr uint64_t prim_root = 7;
  static constexpr int level = 32;
  uint64_t x;
  ModB() : x(0) {}
  ModB(uint64_t n) : x(n >= mod ? n - mod : n) {}
  static constexpr uint64_t modulo() { return mod; }
  static ModB omega() { return ModB(prim_root).pow(mask); }
  uint64_t get() const { return this->x; }
  ModB operator+(ModB rhs) const {
    uint64_t tmp = mod - rhs.x;
    return ModB(this->x >= tmp ? this->x - tmp : this->x + rhs.x);
  }
  ModB operator-(ModB rhs) const {
    return ModB(this->x < rhs.x ? this->x + (mod - rhs.x) : this->x - rhs.x);
  }
  ModB operator*(ModB rhs) const {
    uint64_t lu = this->x >> 32, ld = uint32_t(this->x);
    uint64_t ru = rhs.x >> 32, rd = uint32_t(rhs.x);
    uint64_t a = lu * ru, c = ld * ru, d = ld * rd;
    uint64_t b = lu * rd + (d >> 32);
    uint64_t x = ~b;
    if (c <= x) {
      c += b;
      a += c >> 32;
    } else {
      c -= x + 1;
      a += (c >> 32) + (1ull << 32);
    }
    d = uint32_t(a) * mask + uint32_t(d);
    c = (uint64_t(uint32_t(c)) << 32);
    c = c ? c - (a >> 32) : mod - (a >> 32);
    b = mod - c;
    return (d < b) ? ModB(d + c) : ModB(d - b);
  }
  ModB &operator+=(ModB rhs) { return *this = *this + rhs; }
  ModB &operator-=(ModB rhs) { return *this = *this - rhs; }
  ModB &operator*=(ModB rhs) { return *this = *this * rhs; }
  ModB pow(uint64_t exp) const {
    ModB ret = ModB(1);
    for (ModB base = *this; exp; exp >>= 1, base *= base)
      if (exp & 1) ret *= base;
    return ret;
  }
  ModB inverse() const { return pow(mod - 2); }
  bool operator==(const ModB &p) const { return x == p.x; }
  bool operator!=(const ModB &p) const { return x != p.x; }
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
ModB ff[size], gg[size];
}  // namespace ntt

struct BigInt {
  constexpr static int64_t base = 1000000000, base_digits = 9;

 private:
  bool minus;
  vector<int64_t> dat;

 public:
  BigInt() : minus(false), dat() {}
  BigInt(int64_t v) { *this = v; }
  BigInt(const string &s) { read(s); }

 public:
  void shrink() {
    while (dat.size() && !dat.back()) dat.pop_back();
    if (dat.empty()) minus = false;
  }
  void read(const string &s) {
    minus = false;
    dat.clear();
    int64_t pos = 0;
    while (pos < (int64_t)s.size() && (s[pos] == '-' || s[pos] == '+')) {
      if (s[pos] == '-') minus = !minus;
      ++pos;
    }
    for (int64_t i = s.size() - 1; i >= pos; i -= base_digits) {
      int64_t x = 0;
      for (int64_t j = max(pos, i - base_digits + 1); j <= i; j++)
        x = x * 10 + s[j] - '0';
      dat.push_back(x);
    }
    shrink();
  }
  string to_string() const {
    stringstream ss;
    if (minus) ss << '-';
    ss << (dat.empty() ? 0 : dat.back());
    for (int64_t i = (int64_t)dat.size() - 2; i >= 0; --i)
      ss << setw(base_digits) << setfill('0') << dat[i];
    string ret;
    ss >> ret;
    return ret;
  }
  int convert_int() const { return stoi(this->to_string()); }
  long long convert_ll() const { return stoll(this->to_string()); }
  BigInt &operator=(int64_t v) {
    minus = false;
    dat.clear();
    if (v < 0) minus = true, v = -v;
    for (; v > 0; v = v / base) dat.push_back(v % base);
    return *this;
  }
  bool is_zero() const { return dat.empty() || (dat.size() == 1 && !dat[0]); }
  BigInt operator>>(size_t size) const {
    if (dat.size() <= size) return {};
    BigInt ret;
    ret.dat = vector<int64_t>(dat.begin() + size, dat.end());
    return ret;
  }
  BigInt operator<<(size_t size) const {
    BigInt ret(*this);
    ret.dat.insert(ret.dat.begin(), size, 0);
    return ret;
  }

 private:
  static vector<int64_t> mul_n(const vector<int64_t> &f,
                               const vector<int64_t> &g) {
    vector<int64_t> ret(f.size() + g.size() - 1, 0);
    for (size_t i = 0; i < f.size(); i++)
      for (size_t j = 0; j < g.size(); j++) ret[i + j] += f[i] * g[j];
    return ret;
  }
  static void conv(const vector<int64_t> &f, const vector<int64_t> &g,
                   bool cyclic = false) {
    using namespace ntt;
    for (int i = 0; i < (int)f.size(); i++) ff[i] = f[i];
    if (&f == &g) {
      convolute(ff, f.size(), ff, f.size(), cyclic);
    } else {
      for (int i = 0; i < (int)g.size(); i++) gg[i] = g[i];
      convolute(ff, f.size(), gg, g.size(), cyclic);
    }
  }
  static vector<int64_t> convert_base(const vector<int64_t> &a,
                                      int64_t old_digits, int64_t new_digits) {
    vector<int64_t> p(max(old_digits, new_digits) + 1);
    p[0] = 1;
    for (int64_t i = 1; i < (int64_t)p.size(); i++) p[i] = p[i - 1] * 10;
    vector<int64_t> res;
    int64_t cur = 0;
    int64_t cur_digits = 0;
    for (int64_t i = 0; i < (int64_t)a.size(); i++) {
      cur += a[i] * p[cur_digits];
      cur_digits += old_digits;
      while (cur_digits >= new_digits) {
        res.push_back((cur % p[new_digits]));
        cur /= p[new_digits];
        cur_digits -= new_digits;
      }
    }
    res.push_back(cur);
    while (!res.empty() && !res.back()) res.pop_back();
    return res;
  }
  BigInt mul(const BigInt &v) const {
    if (this->is_zero() || v.is_zero()) return BigInt();
    constexpr static int64_t nbase = 10000, nbase_digits = 4;
    vector<int64_t> f = convert_base(this->dat, base_digits, nbase_digits);
    vector<int64_t> g = convert_base(v.dat, base_digits, nbase_digits);
    while (f.size() < g.size()) f.push_back(0);
    while (g.size() < f.size()) g.push_back(0);
    while (f.size() & (f.size() - 1)) f.push_back(0), g.push_back(0);
    vector<int64_t> h;
    if (f.size() + g.size() < 750 || f.size() < 8 || g.size() < 8) {
      h = mul_n(f, g);
    } else {
      using namespace ntt;
      h.resize(f.size() + g.size() - 1);
      conv(f, g, false);
      for (size_t i = 0; i < h.size(); i++) h[i] = ff[i].get();
    }
    BigInt res = 0;
    res.minus = minus ^ v.minus;
    for (int64_t i = 0, carry = 0; i < (int64_t)h.size(); i++) {
      int64_t cur = h[i] + carry;
      res.dat.push_back((int64_t)(cur % nbase));
      carry = (int64_t)(cur / nbase);
      if (i + 1 == (int)h.size() && carry > 0) h.push_back(0);
    }
    res.dat = convert_base(res.dat, nbase_digits, base_digits);
    res.shrink();
    return res;
  }
  static pair<BigInt, BigInt> divmod(const BigInt &a1, const BigInt &b1) {
    int64_t norm = base / (b1.dat.back() + 1);
    BigInt a = a1.abs() * norm;
    BigInt b = b1.abs() * norm;
    BigInt q, r;
    q.dat.resize(a.dat.size());
    for (int64_t i = a.dat.size() - 1; i >= 0; i--) {
      r *= base;
      r += a.dat[i];
      int64_t s1 = r.dat.size() <= b.dat.size() ? 0 : r.dat[b.dat.size()];
      int64_t s2
          = r.dat.size() <= b.dat.size() - 1 ? 0 : r.dat[b.dat.size() - 1];
      int64_t d = ((int64_t)base * s1 + s2) / b.dat.back();
      r -= b * d;
      while (r < 0) r += b, --d;
      q.dat[i] = d;
    }
    q.minus = a1.minus ^ b1.minus;
    r.minus = a1.minus;
    q.shrink(), r.shrink();
    return make_pair(q, r / norm);
  }
  BigInt quo(const BigInt &b) const {
    size_t preci = dat.size() - b.dat.size();
    BigInt t(1);
    BigInt pre;
    size_t lim = min(int(preci), 3);
    size_t blim = min(int(b.dat.size()), 6);
    t = t << lim;
    while (pre != t) {
      BigInt rb = b >> (b.dat.size() - blim);
      if (blim != b.dat.size()) rb += BigInt(1);
      pre = t;
      t *= (BigInt(2) << (blim + lim)) - rb * t;
      t.dat = vector<int64_t>(t.dat.begin() + lim + blim, t.dat.end());
    }
    if (lim != preci) {
      pre = BigInt();
      while (pre != t) {
        BigInt rb = b >> (b.dat.size() - blim);
        if (blim != b.dat.size()) rb += BigInt({1});
        pre = t;
        t *= (BigInt(2) << (blim + lim)) - rb * t;
        t.dat = vector<int64_t>(t.dat.begin() + lim + blim, t.dat.end());
        size_t next_lim = min(lim * 2 + 1, preci);
        if (next_lim != lim) t = t << next_lim - lim;
        size_t next_blim = min(blim * 2 + 1, b.dat.size());
        lim = next_lim;
        blim = next_blim;
      }
    }
    BigInt ret = (*this) * t;
    ret.dat = vector<int64_t>(ret.dat.begin() + dat.size(), ret.dat.end());
    while ((ret + BigInt(1)) * b <= (*this)) ret += BigInt(1);
    ret.shrink();
    return ret;
  }

 public:
  bool operator<(const BigInt &v) const {
    if (minus != v.minus) return minus;
    if (dat.size() != v.dat.size()) return (dat.size() < v.dat.size()) ^ minus;
    for (int64_t i = dat.size() - 1; i >= 0; i--)
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
    stream << v.to_string();
    return stream;
  }
  BigInt abs() const {
    BigInt res = *this;
    res.minus = false;
    return res;
  }
  BigInt operator-() const {
    BigInt res = *this;
    res.minus = !res.minus;
    return res;
  }
  BigInt &operator*=(int64_t v) {
    if (v < 0) minus = !minus, v = -v;
    for (int64_t i = 0, carry = 0; i < (int64_t)dat.size() || carry; ++i) {
      if (i == (int64_t)dat.size()) dat.push_back(0);
      int64_t cur = dat[i] * (int64_t)v + carry;
      carry = (int64_t)(cur / base);
      dat[i] = (int64_t)(cur % base);
    }
    shrink();
    return *this;
  }
  BigInt operator*(int64_t v) const { return BigInt(*this) *= v; }
  BigInt &operator/=(int64_t v) {
    if (v < 0) minus = !minus, v = -v;
    for (int64_t i = (int64_t)dat.size() - 1, rem = 0; i >= 0; --i) {
      int64_t cur = dat[i] + rem * (int64_t)base;
      dat[i] = (int64_t)(cur / v);
      rem = (int64_t)(cur % v);
    }
    shrink();
    return *this;
  }
  BigInt operator/(int64_t v) const { return BigInt(*this) /= v; }
  int64_t operator%(int64_t v) const {
    assert(v > 0 && !minus);
    int64_t ret = 0;
    for (int64_t i = dat.size() - 1; i >= 0; --i)
      ret = (dat[i] + ret * (int64_t)base) % v;
    return ret;
  }
  BigInt operator+(const BigInt &v) const {
    if (minus != v.minus) return *this - (-v);
    BigInt res = v;
    for (int64_t i = 0, carry = 0;
         i < (int64_t)max(dat.size(), v.dat.size()) || carry; ++i) {
      if (i == (int64_t)res.dat.size()) res.dat.push_back(0);
      res.dat[i] += carry + (i < (int64_t)dat.size() ? dat[i] : 0);
      carry = res.dat[i] >= base;
      if (carry) res.dat[i] -= base;
    }
    return res;
  }
  BigInt operator-(const BigInt &v) const {
    if (minus != v.minus) return *this + (-v);
    if (abs() < v.abs()) return -(v - *this);
    BigInt res = *this;
    for (int64_t i = 0, carry = 0; i < (int64_t)v.dat.size() || carry; ++i) {
      res.dat[i] -= carry + (i < (int64_t)v.dat.size() ? v.dat[i] : 0);
      carry = res.dat[i] < 0;
      if (carry) res.dat[i] += base;
    }
    res.shrink();
    return res;
  }
  BigInt operator*(const BigInt &v) const { return this->mul(v); }
  BigInt operator/(const BigInt &v) const {
    if (*this < v) return BigInt(0);
    return dat.size() < 730 ? divmod(*this, v).first : quo(v);
  }
  BigInt operator%(const BigInt &v) const {
    if (*this < v) return *this;
    return dat.size() < 730 ? divmod(*this, v).second : *this - v * quo(v);
  }
  BigInt &operator+=(const BigInt &v) { return *this = *this + v; }
  BigInt &operator-=(const BigInt &v) { return *this = *this - v; }
  BigInt &operator*=(const BigInt &v) { return *this = *this * v; }
  BigInt &operator/=(const BigInt &v) { return *this = *this / v; }
  BigInt &operator%=(const BigInt &v) { return *this = *this % v; }
};