#define PROBLEM "https://judge.yosupo.jp/problem/polynomial_composite_set_power_series"
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/SetPowerSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int M, N;
 cin >> M >> N;
 int S= 1 << N;
 vector<Mint> a(M), b(S);
 for (int i= 0; i < M; ++i) cin >> a[i];
 for (int i= 0; i < S; ++i) cin >> b[i];
 auto c= SetPowerSeries<20>::polynomial_composite(b, a);
 for (int i= 0; i < S; ++i) cout << c[i] << " \n"[i == S - 1];
 return 0;
}