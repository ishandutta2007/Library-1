// competitive-verifier: PROBLEM https://atcoder.jp/contests/arc182/tasks/arc182_c
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
 long long N, M;
 cin >> N >> M;
 if (M == 1) return cout << Mint(N) << '\n', 0;
 vector<int> p= {2, 3, 5, 7, 11, 13};
 vector<Mint> f(1 << 6);
 for (int i= 1; i <= M; ++i) {
  vector<Mint> g(1 << 6);
  g[0]= 1;
  for (int j= 0, n= i; j < 6; ++j) {
   Mint m= 0;
   while (n % p[j] == 0) n/= p[j], m+= 1;
   for (int J= 1 << j, s= J; s--;) g[s | J]= g[s] * m;
  }
  for (int s= 1 << 6; s--;) f[s]+= g[s];
 }
 auto g= sps::pow(f, N + 1);
 for (int s= 1 << 6; s--;) g[s]-= f[s];
 Mint iv= Mint(1) / (M - 1);
 f[0]= 1;
 for (int s= 1 << 6; --s;) f[s]*= iv;
 for (int s= 1 << 6; s--;) g[s]*= iv;
 g= sps::div(g, f);
 sps::subset_zeta(g);
 cout << g.back() << '\n';
 return 0;
}