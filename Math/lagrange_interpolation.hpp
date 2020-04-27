/**
 * @title ラグランジュ補間
 * @category 数学
 * @brief x=0,1,..,N-1とy=f(0),f(1),...,f(N-1)が与えられたときのf(t)を計算
 * @brief O(N)
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

// verify用:http://codeforces.com/contest/622/problem/F

template <typename K>
K lagrange_interpolation(vector<K> &y, K t) {
  int n = y.size() - 1;
  vector<K> pro(n + 1, 1), orp(n + 1, 1);
  for (int i = 0; i < n; i++) pro[i + 1] = pro[i] * (t - K(i));
  for (int i = n; i > 0; i--) orp[i - 1] = orp[i] * (t - K(i));
  K fact = K(1);
  for (int i = 1; i <= n; i++) fact *= K(i);
  vector<K> finv(n + 1, 1);
  finv[n] = K(1) / fact;
  for (int i = n; i >= 1; i--) finv[i - 1] = finv[i] * K(i);
  K res(0);
  for (int i = 0; i <= n; i++) {
    K tmp = y[i] * pro[i] * orp[i] * finv[i] * finv[n - i];
    if ((n - i) & 1)
      res -= tmp;
    else
      res += tmp;
  }
  return res;
}
