// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/inv_of_formal_power_series
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/FormalPowerSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 using FPS= FormalPowerSeries<Mint>;
 int N;
 cin >> N;
 vector<Mint> A(N);
 for (int i= 0; i < N; i++) cin >> A[i];
 auto ans= FPS(A).inv();
 for (int i= 0; i < N; i++) cout << ans[i] << " \n"[i == N - 1];
 return 0;
}