// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/bitwise_and_convolution
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/set_power_series.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N;
 cin >> N;
 int S= 1 << N;
 vector<Mint> a(S), b(S);
 for (int i= 0; i < S; i++) cin >> a[i];
 for (int i= 0; i < S; i++) cin >> b[i];
 auto c= sps::and_convolve(a, b);
 for (int i= 0; i < S; i++) cout << c[i] << " \n"[i + 1 == S];
 return 0;
}