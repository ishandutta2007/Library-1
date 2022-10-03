#define PROBLEM \
  "https://judge.yosupo.jp/problem/sum_of_exponential_times_polynomial"
/** @see https://min-25.hatenablog.com/entry/2015/04/24/031413
 */
#include <bits/stdc++.h>
#include "src/Math/Combination.hpp"
#include "src/Math/ModInt.hpp"
#include "src/Math/Sieve.hpp"
#include "src/Math/lagrange_interpolation.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = StaticModInt<998244353>;
  using C = Combination<Mint>;
  long long r, d, n;
  cin >> r >> d >> n;
  if (--n < 0) {
    cout << 0 << endl;
    return 0;
  }
  if (r == 0) {
    cout << (d ? 0 : 1) << '\n';
    return 0;
  }
  vector<Mint> sum(d + 2), rpow(d + 2), pd = Sieve<>::pow_table<Mint>(d + 2, d);
  rpow[0] = 1, sum[0] = rpow[0] * pd[0];
  for (int i = 1; i <= d + 1; i++) rpow[i] = rpow[i - 1] * r;
  for (int i = 1; i <= d + 1; i++) sum[i] = sum[i - 1] + rpow[i] * pd[i];
  Mint ans = 0;
  if (r == 1)
    ans = lagrange_interpolation<Mint>(sum, n);
  else {
    for (int i = 0; i <= d; i++) {
      Mint tmp = C::nCr(d + 1, i + 1) * rpow[d - i] * sum[i];
      ans += (d - i) & 1 ? -tmp : tmp;
    }
    ans /= Mint(1 - r).pow(d + 1);
    vector<Mint> y(d + 1);
    Mint rinv = Mint(1) / r, rinvpow = 1;
    for (int i = 0; i <= d; i++) {
      y[i] = Mint(sum[i] - ans) * rinvpow;
      rinvpow *= rinv;
    }
    ans += Mint(r).pow(n) * lagrange_interpolation<Mint>(y, n);
  }
  cout << ans << '\n';
  return 0;
}