#define PROBLEM "https://yukicoder.me/problems/no/922"
// 森
#include <iostream>
#include <array>
#include <vector>
#include "src/Graph/rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M, Q;
 cin >> N >> M >> Q;
 Tree tree(N);
 for (int i= 0; i < M; ++i) {
  int u, v;
  cin >> u >> v;
  tree.add_edge(--u, --v);
 }
 tree.build();
 vector<int> cnt(N);
 int ans= 0;
 while (Q--) {
  int a, b;
  cin >> a >> b;
  --a, --b;
  if (tree.connected(a, b)) ans+= tree.dist(a, b);
  else ++cnt[a], ++cnt[b];
 }
 using Data= array<int, 2>;
 auto f_ee= [&](const Data &l, const Data &r) { return Data{l[0] + r[0], l[1] + r[1]}; };
 auto f_ve= [&](const Data &d, int, auto) { return Data{d[0], d[1] + d[0]}; };
 auto f_ev= [&](const Data &d, int v) { return Data{d[0] + cnt[v], d[1]}; };
 auto dp= rerooting<Data>(tree, f_ee, f_ve, f_ev, Data{0, 0});
 constexpr int INF= 1 << 30;
 vector<int> mi(N, INF);
 for (int i= 0; i < N; ++i) {
  int u= tree.root(i);
  mi[u]= min(mi[u], dp[i][1]);
 }
 for (int i= 0; i < N; ++i)
  if (mi[i] != INF) ans+= mi[i];
 cout << ans << '\n';
 return 0;
}