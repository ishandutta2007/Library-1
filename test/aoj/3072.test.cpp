#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/3072"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/FFT/fps_inv.hpp"
using namespace std;
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint= StaticModInt<998244353>;
  int N, K, P;
  cin >> N >> K >> P;
  Mint p= Mint(P) / 100, q= Mint(1) - p;
  vector<Mint> f(K, -p * p / N);
  f[0]= p;
  auto g= inv(f);
  Mint ans= 1;
  for (int i= 1; i < K; i++) ans-= g[i] * q;
  cout << ans << '\n';
  return 0;
}