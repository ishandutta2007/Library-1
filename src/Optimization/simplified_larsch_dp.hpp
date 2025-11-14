#pragma once
#include <vector>
#include <limits>
#include "src/Internal/function_traits.hpp"
// dp[i] = min_{j<i} (dp[j] + w(i,j))
// w(i,j) -> monge cost
template <class F> std::vector<result_type_t<F>> simplified_larsch_dp(int n, const F &w) {
 using T= result_type_t<F>;
 std::vector<T> dp(n + 1, std::numeric_limits<T>::max());
 std::vector<int> x(n + 1);
 auto check= [&](int i, int j) {
  if (T cost= dp[j] + w(i, j); dp[i] > cost) dp[i]= cost, x[i]= j;
 };
 auto rec= [&](auto &rec, int l, int r) {
  if (r - l <= 1) return;
  int m= (l + r) / 2;
  for (int i= x[l]; i <= x[r]; ++i) check(m, i);
  rec(rec, l, m);
  for (int i= l + 1; i <= m; ++i) check(r, i);
  rec(rec, m, r);
 };
 return dp[0]= 0, check(n, 0), rec(rec, 0, n), dp;
}