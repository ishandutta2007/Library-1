// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/gcd_convolution
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/NumberTheory/tables.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N;
 cin >> N;
 vector<Mint> a(N + 1), b(N + 1);
 for (int i= 1; i <= N; i++) cin >> a[i];
 for (int i= 1; i <= N; i++) cin >> b[i];
 auto c= gcd_convolve(a, b);
 for (int i= 1; i <= N; i++) cout << c[i] << " \n"[i == N];
 return 0;
}