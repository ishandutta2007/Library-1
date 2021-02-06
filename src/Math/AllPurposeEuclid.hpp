#pragma once
#include <bits/stdc++.h>
/**
 * @title 万能欧几里得
 * @category 数学
 * @see https://www.luogu.com.cn/blog/ix-35/solution-p5170
 * @see
 * https://kewth.github.io/2020/07/19/%E4%B8%87%E8%83%BD%E6%AC%A7%E5%87%A0%E9%87%8C%E5%BE%97/
 * @see https://www.cnblogs.com/p-b-p-b/p/13070809.html
 */

// verify用:
// https://loj.ac/p/138
// https://loj.ac/p/6440

// BEGIN CUT HERE

template <typename M>
class AllPurposeEuclid {
  using Node = typename M::Node;
  using u64 = unsigned long long;
  static inline Node pow(Node x, u64 e) {
    Node ret = M::ti();
    for (; e; e >>= 1, x = M::f(x, x))
      if (e & 1) ret = M::f(ret, x);
    return ret;
  }
  static Node rec(u64 n, u64 a, u64 b, u64 c, const Node &sU, const Node &sR) {
    if (!n) return M::ti();
    if (a >= c) return rec(n, a % c, b, c, sU, M::f(pow(sU, a / c), sR));
    u64 m = ((long double)a * n + b) / c;
    if (!m) return pow(sR, n);
    u64 cnt = n - u64(((long double)c * m - b - 1) / a);
    return M::f(M::f(pow(sR, (c - b - 1) / a), sU),
                M::f(rec(m - 1, c, (c - b - 1) % a, a, sR, sU), pow(sR, cnt)));
  }

 public:
  static Node solve(u64 n, u64 a, u64 b, u64 c) {
    return M::f(pow(M::sU, b / c), rec(n, a, b % c, c, M::sU, M::sR));
  }
};

template <typename int_t, int MXK1, int MXK2>
struct FloorSumTable {
  static constexpr int MXK = std::max(MXK1, MXK2) + 1;
  static inline int_t C[MXK][MXK] = {}, pwX[MXK1 + 1] = {1},
                      pwY[MXK2 + 1] = {1};
  static inline int k1 = MXK1, k2 = MXK2;
  using u64 = unsigned long long;
  struct Monoid {
    struct Node {
      int_t cntU = 0, cntR = 0, v[MXK1 + 1][MXK2 + 1] = {0};
    };
    static inline Node sU, sR;
    static Node ti() { return Node(); }
    static Node f(Node vl, const Node &vr) {
      for (int i = 0; i < k1; i++) pwX[i + 1] = pwX[i] * vl.cntR;
      for (int j = 0; j < k2; j++) pwY[j + 1] = pwY[j] * vl.cntU;
      vl.cntU += vr.cntU, vl.cntR += vr.cntR;
      for (int i = 0; i <= k1; i++)
        for (int j = 0; j <= k2; j++)
          for (int k = 0; k <= i; k++)
            for (int l = 0; l <= j; l++)
              vl.v[i][j]
                  += pwX[k] * pwY[l] * C[i][k] * C[j][l] * vr.v[i - k][j - l];
      return vl;
    }
  };
  static void init() {
    for (int i = 0; i < MXK; i++) C[i][0] = 1;
    for (int i = 1; i < MXK; i++)
      for (int j = 1; j <= i; j++) C[i][j] = C[i - 1][j] + C[i - 1][j - 1];
    Monoid::sU.cntU = Monoid::sR.cntR = 1;
    for (int i = 0; i <= k1; i++) Monoid::sR.v[i][0] = 1;
  }
  static auto solve(u64 n, u64 a, u64 b, u64 c, int k1_, int k2_) {
    k1 = k1_, k2 = k2_;
    auto tmp = AllPurposeEuclid<Monoid>::solve(n, a, b, c);
    std::array<std::array<int_t, MXK2 + 1>, MXK1 + 1> ret;
    for (int i = 0; i <= k1; i++)
      for (int j = 0; j <= k2; j++) ret[i][j] = tmp.v[i][j];
    int_t pw = 1, bs = double(b) / c;
    for (int j = 0; j <= k2; j++, pw *= bs) ret[0][j] += pw;
    return ret;
  }
};

template <class R_t, R_t (*ro)(), R_t (*ri)()>
struct RingFloorSum {
  using u64 = unsigned long long;
  struct Monoid {
    struct Node {
      R_t u = ri(), r = ri(), v = ro();
    };
    static inline Node sU, sR;
    static Node ti() { return Node(); }
    static Node f(Node vl, const Node &vr) {
      vl.v += vl.r * vr.v * vl.u, vl.u *= vr.u, vl.r *= vr.r;
      return vl;
    }
  };
  static R_t solve(u64 n, u64 a, u64 b, u64 c, const R_t &A, const R_t &B) {
    Monoid::sU = {B, ri(), ro()}, Monoid::sR = {ri(), A, A};
    return AllPurposeEuclid<Monoid>::solve(n, a, b, c).v;
  }
};
