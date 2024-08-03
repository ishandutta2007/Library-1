// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/UOA/UAPC/3506
#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>
#include "src/Misc/compress.hpp"
#include "src/DataStructure/UnionFind.hpp"
#include "src/Graph/RangeToRangeGraph.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 int x[N], y[N];
 for (int i= 0; i < N; ++i) cin >> x[i] >> y[i];
 vector vx(x, x + N), vy(y, y + N);
 auto idx= compress(vx);
 auto idy= compress(vy);
 for (int i= 0; i < N; ++i) x[i]= idx(x[i]), y[i]= idy(y[i]);
 int X= vx.size(), Y= vy.size();
 vector<int> x2i[X], y2i[Y];
 for (int i= 0; i < N; ++i) x2i[x[i]].push_back(i), y2i[y[i]].push_back(i);
 UnionFind uf(N);
 for (int z= X; z--;)
  for (int i= x2i[z].size(); --i;) uf.unite(x2i[z][i], x2i[z][i - 1]);
 for (int z= Y; z--;)
  for (int i= y2i[z].size(); --i;) uf.unite(y2i[z][i], y2i[z][i - 1]);
 int lx[N], rx[N], ly[N], ry[N];
 fill_n(lx, N, N + 1), fill_n(ly, N, N + 1);
 fill_n(rx, N, -1), fill_n(ry, N, -1);
 for (int i= 0; i < N; ++i) {
  int r= uf.root(i);
  lx[r]= min(lx[r], x[i]);
  rx[r]= max(rx[r], x[i]);
  ly[r]= min(ly[r], y[i]);
  ry[r]= max(ry[r], y[i]);
 }
 RangeToRangeGraph<int> r2r(N + X + Y);
 for (int i= 0; i < N; ++i) {
  if (rx[i] == -1) continue;
  r2r.add_from_range(N + lx[i], N + rx[i] + 1, i, 0);
  r2r.add_to_range(i, N + lx[i], N + rx[i] + 1, 1);
  r2r.add_from_range(N + X + ly[i], N + X + ry[i] + 1, i, 0);
  r2r.add_to_range(i, N + X + ly[i], N + X + ry[i] + 1, 1);
 }
 int n= r2r.graph.vertex_size();
 auto adj= r2r.graph.adjacency_edge(1);
 deque<int> dq;
 int s= uf.root(0), g= uf.root(N - 1);
 int dist[n], used[n];
 static constexpr int INF= 1 << 30;
 fill_n(dist, n, INF);
 fill_n(used, n, 0);
 dist[s]= 0;
 dq.push_back(s);
 while (!dq.empty()) {
  int v= dq.front();
  dq.pop_front();
  if (v == g) break;
  if (used[v]) continue;
  used[v]= true;
  for (int e: adj[v]) {
   int u= r2r.graph[e].to(v);
   int w= r2r.weight[e];
   if (dist[u] > dist[v] + w) {
    dist[u]= dist[v] + w;
    if (w) dq.push_back(u);
    else dq.push_front(u);
   }
  }
 }
 cout << (dist[g] == INF ? -1 : dist[g]) << '\n';
 return 0;
}