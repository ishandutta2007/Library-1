// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/convolution_mod_large
// competitive-verifier: TLE 10
// competitive-verifier: MLE 2048
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/convolve.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N, M;
 cin >> N >> M;
 vector<Mint> a(N), b(M);
 for (int i= 0; i < N; i++) cin >> a[i];
 for (int j= 0; j < M; j++) cin >> b[j];
 auto c= convolve<Mint, (1 << 25)>(a, b);
 c.resize(N + M - 1);
 for (int k= 0; k < N + M - 1; k++) {
  cout << c[k] << " \n"[k == N + M - 2];
 }
 return 0;
}