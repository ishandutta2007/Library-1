// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc201/tasks/abc201_e
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// bitwise xor
#include <iostream>
#include <algorithm>
#include "src/DataStructure/UnionFind_Potentialized.hpp"
#include "src/Math/Nimber.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 Nimber::init();
 int N;
 cin >> N;
 UnionFind_Potentialized<Nimber> uf(N);
 for (int i= N - 1; i--;) {
  int u, v;
  Nimber w;
  cin >> u >> v >> w;
  uf.unite(--u, --v, w);
 }
 Mint ans= 0;
 int cnt[60];
 fill_n(cnt, 60, 0);
 for (int i= N; i--;) {
  long long x= uf.potential(i).val();
  for (int k= 60; k--;) cnt[k]+= (x >> k) & 1;
 }
 for (int k= 60; k--;) ans+= Mint(cnt[k]) * (N - cnt[k]) * (1ll << k);
 cout << ans << '\n';
 return 0;
}