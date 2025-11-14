// competitive-verifier: PROBLEM https://www2.ioi-jp.org/camp/2019/2019-sp-tasks/day3/designated.pdf
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include <vector>
#include <queue>
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Graph g(N, N - 1);
 vector<int> C(N - 1), D(N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i] >> C[i] >> D[i], --g[i];
 auto adje= g.adjacency_edge(0);
 vector<int> deg(N);
 for (int v= N; v--;) deg[v]= adje[v].size();
 priority_queue<pair<long long, int>> pq;
 for (int v= N; v--;)
  if (deg[v] == 1) {
   deg[v]= 0;
   int e= adje[v][0];
   long long x= g[e].first == v ? D[e] : C[e];
   pq.emplace(-x, g[e].to(v));
  }
 vector<long long> ans(N + 1);
 while (pq.size() > 2) {
  auto [c, v]= pq.top();
  pq.pop(), c= -c;
  if (--deg[v] == 1) {
   deg[v]= 0;
   for (int e: adje[v]) {
    int u= g[e].to(v);
    if (deg[u] == 0) continue;
    long long x= g[e].first == v ? D[e] : C[e];
    pq.emplace(-c - x, u);
    break;
   }
  } else ans[pq.size()]= ans[pq.size() + 1] + c;
 }
 auto put_edge= [&](int v, int e, long long x) { return x + (g[e].first == v ? C[e] : D[e]); };
 auto op= [](long long l, long long r) { return l + r; };
 auto put_vertex= [](int, long long x) { return x; };
 ans[1]= 1ll << 61;
 for (auto x: Rerooting<long long>(g, adje, put_edge, op, 0ll, put_vertex)) ans[1]= min(ans[1], x);
 int Q;
 cin >> Q;
 while (Q--) {
  int E;
  cin >> E;
  cout << ans[E] << '\n';
 }
 return 0;
}