#pragma once
#include <bits/stdc++.h>
/**
 * @title ラグランジュ補間
 * @category 数学
 *  x=0,1,..,N-1とy=f(0),f(1),...,f(N-1)が与えられたときのf(t)を計算
 *  O(N)
 */

// verify用:
// http://codeforces.com/contest/622/problem/F
// https://atcoder.jp/contests/arc033/tasks/arc033_4

// BEGIN CUT HERE

template <typename K>
K lagrange_interpolation(std::vector<K> &y, K t) {
  int n = y.size() - 1;
  std::vector<K> pro(n + 1, 1), orp(n + 1, 1);
  for (int i = 0; i < n; i++) pro[i + 1] = pro[i] * (t - K(i));
  for (int i = n; i > 0; i--) orp[i - 1] = orp[i] * (t - K(i));
  K fact = K(1);
  for (int i = 1; i <= n; i++) fact *= K(i);
  std::vector<K> finv(n + 1, 1);
  finv[n] = K(1) / fact;
  for (int i = n; i >= 1; i--) finv[i - 1] = finv[i] * K(i);
  K res(0);
  for (int i = 0; i <= n; i++) {
    K tmp = y[i] * pro[i] * orp[i] * finv[i] * finv[n - i];
    res += (n - i) & 1 ? -tmp : tmp;
  }
  return res;
}
