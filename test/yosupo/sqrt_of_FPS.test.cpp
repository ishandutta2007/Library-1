// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/sqrt_of_formal_power_series
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/fps_sqrt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 using Mint= ModInt<998244353>;
 vector<Mint> a(N);
 for (int i= 0; i < N; i++) cin >> a[i];
 auto b= sqrt(a);
 if (b.size() == 0) {
  cout << -1 << endl;
 } else {
  for (int i= 0; i < N; i++) cout << b[i] << " \n"[i == N - 1];
 }
 return 0;
}