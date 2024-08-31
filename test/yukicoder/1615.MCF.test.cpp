// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1615
// competitive-verifier: TLE 0.5
// 重み付き二部マッチング(非想定解)
// 制約が厳しい
#include <iostream>
#include "src/Optimization/NetworkSimplex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, M, K, L;
 cin >> N >> M >> K >> L;
 NetworkSimplex<int, long long, MAXIMIZE> graph;
 int gs= graph.add_vertex(), gt= graph.add_vertex();
 auto buyer= graph.add_vertices(N);
 auto product= graph.add_vertices(M);
 for (int i= 0; i < L; i++) {
  int X, Y, Z;
  cin >> X >> Y >> Z, X--, Y--;
  graph.add_edge(buyer[X], product[Y], 0, 1, (1ll << Z));
 }
 for (int i= 0; i < N; i++) graph.add_edge(gs, buyer[i], 0, 1, 0);
 for (int j= 0; j < M; j++) graph.add_edge(product[j], gt, 0, 1, 0);
 graph.add_edge(gs, gt, 0, N + M, 0);
 graph.add_supply(gs, N + M), graph.add_demand(gt, N + M);
 graph.b_flow();
 cout << graph.get_result_value() << '\n';
 return 0;
}