// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/log_of_formal_power_series
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/FormalPowerSeries.hpp"
using namespace std;
signed main() {
 using Mint= ModInt<998244353>;
 using FPS= FormalPowerSeries<Mint>;
 int N;
 cin >> N;
 std::vector<Mint> A(N);
 for (int i= 0; i < N; i++) cin >> A[i];
 auto ans= log(FPS(A));
 for (int i= 0; i < N; i++) cout << ans[i] << " \n"[i == N - 1];
}