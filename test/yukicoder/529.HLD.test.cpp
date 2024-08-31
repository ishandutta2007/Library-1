// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/529
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include "src/Misc/compress.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/IncrementalBridgeConnectivity.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/DataStructure/SegmentTree.hpp"
using namespace std;
struct RmaxQ {
 using T= pair<long long, int>;
 static T ti() { return {-1, -1}; }
 static T op(const T &vl, const T &vr) { return vl.first > vr.first ? vl : vr; }
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

 Graph tree(n);
 for (auto [u, v]: g) {
  u= id[ibc.leader(u)], v= id[ibc.leader(v)];
  if (u == v) continue;
  if (u > v) swap(u, v);
  tree.add_edge(u, v);
 }
 compress(tree);
 HeavyLightDecomposition hld(tree);
 SegmentTree<RmaxQ> seg(n);
 for (int i= n; i--;) {
  int v= hld.to_vertex(i);
  seg.set(i, {-1, v});
 }

 priority_queue<long long> pq[n];
 for (int v= n; v--;) pq[v].push(-1);
 while (Q--) {
  int op, x, y;
  cin >> op >> x >> y;
  if (op == 1) {
   int u= id[ibc.leader(--x)];
   pq[u].push(y);
   int i= hld.to_seq(u);
   seg.set(i, make_pair(pq[u].top(), u));
  } else {
   int u= id[ibc.leader(--x)], v= id[ibc.leader(--y)];
   long long ans= -1;
   int w;
   for (auto [l, r]: hld.path(u, v)) {
    auto [a, b]= l < r ? seg.fold(l, r + 1) : seg.fold(r, l + 1);
    if (ans < a) ans= a, w= b;
   }
   cout << ans << '\n';
   if (ans != -1) {
    pq[w].pop();
    int i= hld.to_seq(w);
    seg.set(i, make_pair(pq[w].top(), w));
   }
  }
 }
 return 0;
}