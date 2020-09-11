/**
 * @title Rolling-Hash
 * @category 文字列
 *  Mod は　2^61-1 で固定
 * @see https://qiita.com/keymoon/items/11fac5627672a6d6a9f6
 */

// lcp_concat https://atcoder.jp/contests/arc050/tasks/arc050_d

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

struct RollingHash {
 private:
  static const uint64_t mod = (1ull << 61ull) - 1;
  vector<uint64_t> hash, pw;
  const uint64_t base;

 private:
  static inline uint64_t add(uint64_t a, uint64_t b) {
    if ((a += b) >= mod) a -= mod;
    return a;
  }
  static inline uint64_t mul(uint64_t a, uint64_t b) {
    __uint128_t c = (__uint128_t)a * b;
    return add(c >> 61, c & mod);
  }

 public:
  static inline uint64_t generate_base() {
    mt19937_64 mt(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<uint64_t> rand(1, RollingHash::mod - 1);
    return rand(mt);
  }

  RollingHash() = default;
  RollingHash(const string &s, uint64_t base)
      : RollingHash(vector<char>(s.begin(), s.end()), base) {}
  template <typename T>
  RollingHash(const vector<T> &s, uint64_t base) : base(base) {
    hash.assign(s.size() + 1, 0);
    pw.assign(s.size() + 1, 0);
    pw[0] = 1;
    for (size_t i = 0; i < s.size(); i++) {
      pw[i + 1] = mul(pw[i], base);
      hash[i + 1] = add(mul(hash[i], base), s[i]);
    }
  }
  // S[l, r)
  uint64_t get_hash(int l, int r) const {
    return add(hash[r], mod - mul(hash[l], pw[r - l]));
  }
  uint64_t concat_hash(int l1, int r1, int l2, int r2) const {
    return concat_hash(*this, *this, l1, r1, l2, r2);
  }
  static uint64_t concat_hash(const RollingHash &a, const RollingHash &b,
                              int l1, int r1, int l2, int r2) {
    assert(a.base == b.base);
    return add(mul(a.get_hash(l1, r1), b.pw[r2 - l2]), b.get_hash(l2, r2));
  }
};

struct RollingHash_MultiBase {
  vector<RollingHash> rhs;
  vector<uint64_t> bases;
  RollingHash_MultiBase() = default;
  RollingHash_MultiBase(const string &s, vector<uint64_t> bs)
      : RollingHash_MultiBase(vector<char>(s.begin(), s.end()), bs) {}
  template <typename T>
  RollingHash_MultiBase(const vector<T> &s, vector<uint64_t> bs) : bases(bs) {
    for (auto base : bases) rhs.emplace_back(RollingHash(s, base));
  }
  bool equal(int l1, int r1, int l2, int r2) const {
    return equal(*this, *this, l1, r1, l2, r2);
  }
  int lcp(int l1, int r1, int l2, int r2) const {
    return lcp(*this, *this, l1, r1, l2, r2);
  }
  int lcp_concat(int l1, int r1, int l2, int r2) const {
    return lcp_concat(*this, *this, l1, r1, l2, r2);
  }
  static bool equal(const RollingHash_MultiBase &a,
                    const RollingHash_MultiBase &b, int l1, int r1, int l2,
                    int r2) {
    assert(a.bases.size() == b.bases.size());
    for (size_t i = 0; i < a.bases.size(); i++)
      if (a.rhs[i].get_hash(l1, r1) != b.rhs[i].get_hash(l2, r2)) return false;
    return true;
  }
  static int lcp(const RollingHash_MultiBase &a, const RollingHash_MultiBase &b,
                 int l1, int r1, int l2, int r2) {
    int len = min(r1 - l1, r2 - l2);
    int low = 0, high = len + 1;
    while (high - low > 1) {
      int mid = (low + high) / 2;
      if (equal(a, b, l1, l1 + mid, l2, l2 + mid))
        low = mid;
      else
        high = mid;
    }
    return low;
  }
  // lcp of s+t and t+s
  static int lcp_concat(const RollingHash_MultiBase &a,
                        const RollingHash_MultiBase &b, int l1, int r1, int l2,
                        int r2) {
    auto eq = [&](int x) {
      for (size_t i = 0; i < a.bases.size(); i++) {
        uint64_t ha = l1 + x <= r1
                          ? a.rhs[i].get_hash(l1, l1 + x)
                          : RollingHash::concat_hash(a.rhs[i], b.rhs[i], l1, r1,
                                                     l2, l2 + x + l1 - r1);
        uint64_t hb = l2 + x <= r2
                          ? b.rhs[i].get_hash(l2, l2 + x)
                          : RollingHash::concat_hash(b.rhs[i], a.rhs[i], l2, r2,
                                                     l1, l1 + x + l2 - r2);
        if (ha != hb) return false;
      }
      return true;
    };
    int len = r1 - l1 + r2 - l2;
    int low = 0, high = len + 1;
    while (high - low > 1) {
      int mid = (low + high) / 2;
      if (eq(mid))
        low = mid;
      else
        high = mid;
    }
    return low;
  }
};