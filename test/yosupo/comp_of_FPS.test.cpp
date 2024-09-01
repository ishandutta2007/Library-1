// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/composition_of_formal_power_series
// competitive-verifier: TLE 1
// competitive-verifier: MLE 256
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/FFT/Polynomial.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N;
 cin >> N;
 Polynomial<Mint> a(N), b(N);
 for (int i= 0; i < N; i++) cin >> a[i];
 for (int i= 0; i < N; i++) cin >> b[i];
 auto c= a(b);
 for (int i= 0; i < N; i++) cout << c[i] << " \n"[i == N - 1];
 return 0;
}