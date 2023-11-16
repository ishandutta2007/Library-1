#pragma once
#include <vector>
template <class T, class F> struct SparseTable {
 std::vector<std::vector<T>> dat;
 const F &f;
 SparseTable(const std::vector<T> &v, const F &f): f(f) {
  int n= v.size(), log= n > 1 ? 31 - __builtin_clz(n - 1) : 0;
  dat.resize(log + 1), dat[0].assign(v.begin(), v.end());
  for (int i= 0, I= 1, j; i < log; ++i, I<<= 1)
   for (dat[i + 1].resize(j= dat[i].size() - I); j--;) dat[i + 1][j]= f(dat[i][j], dat[i][j + I]);
 }
 // [l, r)
 T fold(int l, int r) {
  if (r == l + 1) return dat[0][l];
  int k= 31 - __builtin_clz(r - l - 1);
  return f(dat[k][l], dat[k][r - (1 << k)]);
 }
};