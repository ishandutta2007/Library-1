// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/7/GRL_7_A
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// 200 (=100+100) 頂点
#include <iostream>
#include "src/Graph/CliqueProblem.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int X, Y, E;
 cin >> X >> Y >> E;
 CliqueProblem graph(X + Y);
 for (int i= 0; i < E; ++i) {
  int x, y;
  cin >> x >> y;
  graph.add_edge(x, X + y);
 }
 cout << graph.get_min_vertex_cover().size() << '\n';
 return 0;
}