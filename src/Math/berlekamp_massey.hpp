#pragma once
#include <bits/stdc++.h>
// a[n] = c[0] * a[n-1] + c[1] * a[n-2] + ... + c[d-1] * a[n-d]
// return c
template <class K> std::vector<K> berlekamp_massey(const std::vector<K> &a) {
 std::size_t n= a.size(), d= 0, m= 0, i, j;
 if (n == 0) return {};
 std::vector<K> c(n), b(n), tmp;
 K x= 1, y, coef;
 const K Z= 0;
 for (c[0]= b[0]= 1, i= 0, j; i < n; ++i) {
  for (++m, y= a[i], j= 1; j <= d; ++j) y+= c[j] * a[i - j];
  if (y == Z) continue;
  for (tmp= c, coef= y / x, j= m; j < n; ++j) c[j]-= coef * b[j - m];
  if (2 * d > i) continue;
  d= i + 1 - d, b= tmp, x= y, m= 0;
 }
 c.resize(d + 1), c.erase(c.begin());
 for (auto &x: c) x= -x;
 return c;
}