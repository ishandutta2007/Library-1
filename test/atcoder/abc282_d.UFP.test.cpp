// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc282/tasks/abc282_d
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// ポテンシャルUF (2色塗り分け+連結成分)
#include <iostream>
#include "src/DataStructure/UnionFind_Potentialized.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 UnionFind_Potentialized<bool> uf(N);
 bool isok= true;
 for (int i= 0; i < M; ++i) {
  int u, v;
  cin >> u >> v, --u, --v;
  isok&= uf.unite(u, v, 1);
 }
 if (!isok) return cout << 0 << '\n', 0;
 vector cnt(2, vector(N, 0));
 for (int v= N; v--;) ++cnt[uf.potential(v)][uf.leader(v)];
 long long ans= 0;
 for (int s= 2; s--;)
  for (int v= N; v--;) ans+= (long long)cnt[s][v] * (N - cnt[s][v]);
 ans/= 2, ans-= M;
 cout << ans << '\n';
 return 0;
}