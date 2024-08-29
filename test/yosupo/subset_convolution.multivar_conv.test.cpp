// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/subset_convolution
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/MultiVariateConvolution.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N;
 cin >> N;
 MultiVariateConvolution mvc(vector(N, 2));
 vector<Mint> a(1 << N), b(1 << N);
 for (auto &ai: a) cin >> ai;
 for (auto &bi: b) cin >> bi;
 auto c= mvc.convolve<Mint, 1 << 21, 20>(a, b);
 for (int i= 0; i < (1 << N); i++) cout << c[i] << " \n"[i + 1 == 1 << N];
 return 0;
}