/**
 * @title 高速きたまさ法
 * @category 数学
 * @brief O(NlogNlogk)
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/FormalPowerSeries.hpp"
#undef call_from_test
#endif

// b[0] = a[0], b[1] = a[1], ..., b[N-1] = a[N-1]
// b[n] = c[0] * b[n-N] + c[1] * b[n-N+1] + ... + c[N-1] * b[n-1] (n >= N)
// calc b[k]

R kitamasa(const vector<R> &c, const vector<R> &a, u64 k, R mod) {
  assert(a.size() == c.size());
  int N = a.size();
  if (k < N) return a[k];
  if (FPS::mod != mod) FPS::init(mod);
  u64 mask = (u64(1) << (63 - __builtin_clzll(k))) >> 1;
  FPS f(N + 1);
  f[0] = 1;
  for (int i = 0; i < N; i++) f[N - i] = mod - c[i];
  FPS r(vector<R>({1, 0}));
  if (N < 250) {  // naive
    r = r.divrem_rev_n(f).second;
    while (mask) {
      r *= r;
      if (k & mask) r.push_back(0);
      r = r.divrem_rev_n(f).second;
      mask >>= 1;
    }
  } else {
    FPS inv = f.inv(N);
    r = r.rem_rev_pre(f, inv);
    while (mask) {
      r *= r;
      if (k & mask) r.push_back(0);
      r = r.rem_rev_pre(f, inv);
      mask >>= 1;
    }
  }
  R ret = 0;
  for (int i = 0; i < N; i++)
    FPS::mod_add(ret, FPS::mod_mul(r[N - i - 1], a[i]));
  return ret;
}