// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/shift_of_sampling_points_of_polynomial
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 256
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/sample_points_shift.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N, M;
 Mint c;
 cin >> N >> M >> c;
 std::vector<Mint> pts(N);
 for (int i= 0; i < N; i++) cin >> pts[i];
 auto ans= sample_points_shift(pts, c, M);
 for (int i= 0; i < M; i++) cout << ans[i] << " \n"[i == M - 1];
 return 0;
}