// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/3168
#include <iostream>
#include <algorithm>
#include "src/Graph/Graph.hpp"
#include "src/Graph/BipartiteGraph.hpp"
#include "src/Graph/DulmageMendelsohn.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M, K;
 cin >> N >> M >> K;
 char c[N];
 for (int i= 0; i < N; ++i) cin >> c[i];
 int dist[N][N];
 for (int i= 0; i < N; ++i) fill_n(dist[i], N, K + 1);
 for (int i= 0; i < N; ++i) dist[i][i]= 0;
 for (int i= 0; i < M; ++i) {
  int u, v;
  cin >> u >> v;
  --u, --v;
  dist[u][v]= dist[v][u]= 1;
 }
 for (int k= 0; k < N; ++k)
  for (int i= 0; i < N; ++i)
   for (int j= 0; j < N; ++j) dist[i][j]= min(dist[i][j], dist[i][k] + dist[k][j]);
 Graph g(N);
 for (int i= 0; i < N; ++i)
  for (int j= 0; j < i; ++j)
   if (int x= (c[i] - c[j] + 26) % 26; (x == 1 || x == 25) && dist[i][j] <= K) g.add_edge(i, j);
 auto [bg, _, __]= graph_to_bipartite(g);
 DulmageMendelsohn dm(bg);
 cout << dm.min_vertex_cover().size() << '\n';
 return 0;
}