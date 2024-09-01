// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/partition_function
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
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
 auto ans= MSET(FPS(vector<Mint>(N + 1, 1)));
 for (int i= 0; i <= N; i++) cout << ans[i] << " \n"[i == N];
 return 0;
}