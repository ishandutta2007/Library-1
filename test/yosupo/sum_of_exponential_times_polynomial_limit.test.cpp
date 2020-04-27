#define PROBLEM \
  "https://judge.yosupo.jp/problem/sum_of_exponential_times_polynomial_limit"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/Combination.hpp"
#include "Math/ModInt.hpp"
#undef call_from_test

template <class Modint>
vector<Modint> pow_d_list(int n, long long d) {
  vector<int> pdiv(n);
  for (int i = 2; i < n; i++) pdiv[i] = i & 1 ? i : 2;
  for (int p = 3; p * p < n; p += 2)
    if (pdiv[p] == p)
      for (int q = p * p; q < n; q += 2 * p) pdiv[q] = p;

  vector<Modint> res(n);
  if (d == 0) res[0] = 1;
  if (n >= 2) res[1] = 1;
  for (int i = 2; i < n; i++) {
    if (pdiv[i] == i)
      res[i] = Modint(i).pow(d);
    else
      res[i] = res[pdiv[i]] * res[i / pdiv[i]];
  }
  return res;
}

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  using C = Combination<Mint>;
  long long r, d;
  cin >> r >> d;
  vector<Mint> sum(d + 1), rpow(d + 1), pd = pow_d_list<Mint>(d + 1, d);
  rpow[0] = 1, sum[0] = rpow[0] * pd[0];
  for (int i = 1; i <= d; i++) rpow[i] = rpow[i - 1] * r;
  for (int i = 1; i <= d; i++) sum[i] = sum[i - 1] + rpow[i] * pd[i];
  Mint ans = 0;
  C::init(d + 1);
  for (int i = 0; i <= d; i++) {
    if ((d - i) & 1)
      ans -= C::C(d + 1, i + 1) * rpow[d - i] * sum[i];
    else
      ans += C::C(d + 1, i + 1) * rpow[d - i] * sum[i];
  }
  ans /= Mint(1 - r).pow(d + 1);
  cout << ans << endl;
  return 0;
}