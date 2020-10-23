#pragma once
#include <bits/stdc++.h>
/**
 * @title Rolling-Hash
 * @category 文字列
 *  Mod は　2^61-1 で固定
 * @see https://qiita.com/keymoon/items/11fac5627672a6d6a9f6
 */

// lcp verify https://atcoder.jp/contests/arc050/tasks/arc050_d

// BEGIN CUT HERE

struct RollingHash {
 private:
  static constexpr std::uint64_t mod = (1ull << 61ull) - 1;
  std::vector<std::uint64_t> hash, pw;
  std::uint64_t base;

 private:
  static inline std::uint64_t add(std::uint64_t a, std::uint64_t b) {
    if ((a += b) >= mod) a -= mod;
    return a;
  }
  static inline std::uint64_t mul(std::uint64_t a, std::uint64_t b) {
    __uint128_t c = (__uint128_t)a * b;
    return add(c >> 61, c & mod);
  }

 public:
  static inline std::uint64_t generate_base() {
    std::mt19937_64 mt(chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<std::uint64_t> rand(1, RollingHash::mod - 1);
    return rand(mt);
  }

  RollingHash() = default;
  RollingHash(const std::string& s, std::uint64_t base)
      : RollingHash(std::vector<char>(s.begin(), s.end()), base) {}
  template <typename T>
  RollingHash(const std::vector<T>& s, std::uint64_t base) : base(base) {
    hash.assign(s.size() + 1, 0);
    pw.assign(s.size() + 1, 0);
    pw[0] = 1;
    for (size_t i = 0; i < s.size(); i++) {
      pw[i + 1] = mul(pw[i], base);
      hash[i + 1] = add(mul(hash[i], base), s[i]);
    }
  }
  // S[l, r)
  std::uint64_t get_hash(int l = 0, int r = -1) const {
    if (r < 0) r = pw.size() - 1;
    return add(hash[r], mod - mul(hash[l], pw[r - l]));
  }
  std::uint64_t combine_hash(std::uint64_t hash1, std::uint64_t hash2,
                             int hash2len) {
    return add(mul(hash1, pw[hash2len]), hash2);
  }
};

template <size_t SIZE>
struct RollingHash_MultiBase {
  using Array = std::array<std::uint64_t, SIZE>;
  std::array<RollingHash, SIZE> rhs;
  Array bases;
  RollingHash_MultiBase() = default;
  RollingHash_MultiBase(const std::string& s, Array bs)
      : RollingHash_MultiBase(std::vector<char>(s.begin(), s.end()), bs) {}
  template <typename T>
  RollingHash_MultiBase(const std::vector<T>& s, Array bs) : bases(bs) {
    for (size_t i = 0; i < SIZE; i++) rhs[i] = RollingHash(s, bases[i]);
  }
  Array get_hash(int l, int r) const {
    Array ret;
    for (size_t i = 0; i < SIZE; i++) ret[i] = rhs[i].get_hash(l, r);
    return ret;
  }
  bool equal(int l1, int r1, int l2, int r2) const {
    return equal(*this, *this, l1, r1, l2, r2);
  }
  int lcp(int l1, int r1, int l2, int r2) const {
    return lcp(*this, *this, l1, r1, l2, r2);
  }
  static bool equal(const RollingHash_MultiBase& a,
                    const RollingHash_MultiBase& b, int l1, int r1, int l2,
                    int r2) {
    for (size_t i = 0; i < SIZE; i++)
      if (a.rhs[i].get_hash(l1, r1) != b.rhs[i].get_hash(l2, r2)) return false;
    return true;
  }
  static int lcp(const RollingHash_MultiBase& a, const RollingHash_MultiBase& b,
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
};