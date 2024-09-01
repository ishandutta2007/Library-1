// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/3072
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/sparse_fps.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 int N, K, P;
 cin >> N >> K >> P;
 Mint p= Mint(P) / 100, ans= 1, c= (Mint(1) - p) / p;
 vector<Mint> f(N + 2);
 f[0]= N;
 f[1]= -(p + N);
 f[N + 1]= p;
 auto g= sfps::div({c * N, -c * N}, f, K);
 for (int i= 1; i < K; ++i) ans-= g[i];
 cout << ans << '\n';
 return 0;
}