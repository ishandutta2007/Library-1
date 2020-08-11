/**
 * @title Rolling-Hash
 * @category 文字列
 *  Mod は　2^61-1 で固定
 * @see https://qiita.com/keymoon/items/11fac5627672a6d6a9f6
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <unsigned long long B>
struct RollingHash {
 private:
  static constexpr uint64_t MASK30 = (1UL << 30) - 1, MASK31 = (1UL << 31) - 1,
                            MASK61 = (1UL << 61) - 1;
  static constexpr uint64_t MOD = (1UL << 61) - 1;
  vector<uint64_t> hash, po;

 public:
  RollingHash() {}
  RollingHash(vector<long long> vs) { init(vs); }
  RollingHash(string &s) {
    vector<long long> vs;
    for (char c : s) vs.emplace_back(c);
    init(vs);
  }
  void init(vector<long long> vs) {
    int n = vs.size();
    hash.assign(n + 1, 0);
    po.assign(n + 1, 1);
    for (int i = 0; i < n; i++) {
      hash[i + 1] = calcmod(mul(hash[i], B) + vs[i]);
      po[i + 1] = calcmod(mul(po[i], B));
    }
  }
  // S[l, r)
  uint64_t get_hash(int l, int r) {
    uint64_t res = hash[r] + MOD * 3 - mul(hash[l], po[r - l]);
    return calcmod(res);
  }

 private:
  uint64_t mul(uint64_t a, uint64_t b) {
    uint64_t au = a >> 31, ad = a & MASK31;
    uint64_t bu = b >> 31, bd = b & MASK31;
    uint64_t mid = ad * bu + au * bd;
    uint64_t midu = mid >> 30, midd = mid & MASK30;
    return au * bu * 2 + midu + (midd << 31) + ad * bd;
  }

  uint64_t calcmod(uint64_t x) {
    uint64_t res = (x >> 61) + (x & MASK61);
    return res >= MOD ? res - MOD : res;
  }
};