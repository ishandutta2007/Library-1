// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/pow_of_formal_power_series
// competitive-verifier: TLE 2
// competitive-verifier: MLE 128
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/FormalPowerSeries.hpp"
using namespace std;
signed main() {
 using Mint= ModInt<998244353>;
 using FPS= FormalPowerSeries<Mint>;
 long long N, M;
 cin >> N >> M;
 std::vector<Mint> A(N);
 for (int i= 0; i < N; i++) cin >> A[i];
 auto ans= pow(FPS(A), M);
 for (int i= 0; i < N; i++) cout << ans[i] << " \n"[i == N - 1];
}