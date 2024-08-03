// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/529
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include "src/Graph/Graph.hpp"
#include "src/Graph/IncrementalBridgeConnectivity.hpp"
#include "src/DataStructure/LinkCutTree.hpp"
using namespace std;
struct RmaxQ {
 using T= pair<long long, int>;
 static T op(const T &vl, const T &vr) { return vl.first > vr.first ? vl : vr; }
 using commute= void;
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M, Q;
 cin >> N >> M >> Q;
 Graph g(N, M);
 for (int i= 0; i < M; ++i) cin >> g[i], --g[i];
 IncrementalBridgeConnectivity ibc(N);
 for (auto [u, v]: g) ibc.add_edge(u, v);

 vector<int> id(N);
 int n= 0;
 for (int i= 0; i < N; ++i)
  if (i == ibc.leader(i)) id[i]= n++;

 LinkCutTree<RmaxQ> lct(n);
 for (int i= 0; i < n; i++) lct.set(i, {-1, i});
 for (auto [u, v]: g) {
  u= id[ibc.leader(u)], v= id[ibc.leader(v)];
  if (u == v) continue;
  lct.link(u, v);
 }

 priority_queue<long long> pq[n];
 for (int v= n; v--;) pq[v].push(-1);
 while (Q--) {
  int op, x, y;
  cin >> op >> x >> y;
  if (op == 1) {
   int u= id[ibc.leader(--x)];
   pq[u].push(y);
   lct.set(u, make_pair(pq[u].top(), u));
  } else {
   int u= id[ibc.leader(--x)], v= id[ibc.leader(--y)];
   auto [ans, v]= lct.fold(u, v);
   cout << ans << '\n';
   if (ans != -1) {
    pq[v].pop();
    lct.set(v, make_pair(pq[v].top(), v));
   }
  }
 }
 return 0;
}