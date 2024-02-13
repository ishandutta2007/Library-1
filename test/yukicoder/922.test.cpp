#define PROBLEM "https://yukicoder.me/problems/no/922"
// 森
#include <iostream>
#include <array>
#include <vector>
#include "src/Graph/Graph.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M, Q;
 cin >> N >> M >> Q;
 Graph g(M);
 for (int i= 0; i < M; ++i) cin >> g[i], --g[i];
 g.build(N, 0);
 HeavyLightDecomposition forest(g);
 vector<int> cnt(N);
 long long ans= 0;
 while (Q--) {
  int a, b;
  cin >> a >> b;
  --a, --b;
  if (forest.connected(a, b)) ans+= forest.dist(a, b);
  else ++cnt[a], ++cnt[b];
 }
 using Data= array<long long, 2>;
 auto put_edge= [&](int, int, Data d) { return d[1]+= d[0], d; };
 auto op= [&](const Data &l, const Data &r) { return Data{l[0] + r[0], l[1] + r[1]}; };
 auto put_vertex= [&](int v, Data d) { return d[0]+= cnt[v], d; };
 Rerooting<Data> dp(g, forest, put_edge, op, Data{0, 0}, put_vertex);
 constexpr long long INF= 1ll << 60;
 vector<long long> mi(N, INF);
 for (int v= N; v--;) {
  int u= forest.root(v);
  mi[u]= min(mi[u], dp[v][1]);
 }
 for (int i= 0; i < N; ++i)
  if (mi[i] != INF) ans+= mi[i];
 cout << ans << '\n';
 return 0;
}