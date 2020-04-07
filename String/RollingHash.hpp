/**
 * @title RollingHash
 * @category 文字列
 * @brief Mod は　2^61-1 で固定
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <unsigned long long B>
struct RollingHash {
  using ull = unsigned long long;
  const ull MASK30 = (1UL << 30) - 1;
  const ull MASK31 = (1UL << 31) - 1;
  const ull MOD = (1UL << 61) - 1;
  const ull MASK61 = MOD;
  vector<ull> hash, po;
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
      hash[i + 1] = CalcMod(Mul(hash[i], B) + vs[i]);
      po[i + 1] = CalcMod(Mul(po[i], B));
    }
  }
  // a*b mod 2^61-1を返す関数(最後にModを取る)
  ull Mul(ull a, ull b) {
    ull au = a >> 31;
    ull ad = a & MASK31;
    ull bu = b >> 31;
    ull bd = b & MASK31;
    ull mid = ad * bu + au * bd;
    ull midu = mid >> 30;
    ull midd = mid & MASK30;
    return au * bu * 2 + midu + (midd << 31) + ad * bd;
  }

  // mod 2^61-1を計算する関数
  ull CalcMod(ull x) {
    ull xu = x >> 61;
    ull xd = x & MASK61;
    ull res = xu + xd;
    return res >= MOD ? res - MOD : res;
  }
  // S[l, r)
  ull find(int l, int r) {
    ull res = hash[r] + MOD * 3 - Mul(hash[l], po[r - l]);
    return CalcMod(res);
  }
};