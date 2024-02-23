#define PROBLEM "https://atcoder.jp/contests/abc321/tasks/abc321_g"
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/FactorialPrecalculation.hpp"
#include "src/Math/set_power_series.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 using F= FactorialPrecalculation<Mint>;
 int N, M;
 cin >> N >> M;
 int n= 1 << N;
 vector<int> r(n), b(n);
 for (int i= 0, R; i < M; ++i) cin >> R, --R, r[1 << R]+= 1;
 for (int i= 0, B; i < M; ++i) cin >> B, --B, b[1 << B]+= 1;
 sps::subset_zeta(r), sps::subset_zeta(b);
 vector<Mint> f(n);
 for (int s= n; s--;) {
  if (r[s] != b[s]) continue;
  f[s]= F::fact(r[s]);
 }
 auto ans= sps::convolve(f, sps::log(f));
 cout << ans[n - 1] * F::finv(M) << '\n';
 return 0;
}