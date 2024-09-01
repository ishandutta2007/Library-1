// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/pow_of_formal_power_series
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/fps_exp.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long N, M;
 cin >> N >> M;
 using Mint= ModInt<998244353>;
 vector<Mint> a(N);
 for (int i= 0; i < N; i++) cin >> a[i];
 auto b= pow(a, M);
 for (int i= 0; i < N; i++) cout << b[i] << " \n"[i == N - 1];
 return 0;
}