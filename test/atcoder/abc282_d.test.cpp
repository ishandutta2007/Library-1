// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc282/tasks/abc282_d
// competitive-verifier: TLE 0.5
// 2色塗り分けの verify

#include <iostream>
#include <vector>
#include "src/DataStructure/UnionFind.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/BipartiteGraph.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 Graph g(N, M);
 for (int i= 0; i < M; ++i) cin >> g[i], --g[i];
 UnionFind uf(N);
 for (auto [u, v]: g) uf.unite(u, v);
 auto col= paint_two_colors(g);
 if (col.empty()) return cout << 0 << '\n', 0;
 vector cnt(2, vector(N, 0));
 for (int v= N; v--;) ++cnt[col[v]][uf.root(v)];
 long long ans= 0;
 for (int s= 2; s--;)
  for (int v= N; v--;) ans+= (long long)cnt[s][v] * (N - cnt[s][v]);
 ans/= 2, ans-= M;
 cout << ans << '\n';
 return 0;
}