// competitive-verifier: STANDALONE

// https://oj.uz/problem/view/APIO16_fireworks
#include <sstream>
#include <string>
#include <cassert>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
#include "src/Graph/Graph.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
signed main(stringstream& scin, stringstream& scout) {
 using PLC= PiecewiseLinearConvex<long long>;
 int N, M;
 scin >> N >> M;
 Graph g(N + M, N + M - 1);
 vector<int> C(N + M - 1);
 for (int i= 0, p; i < N + M - 1; ++i) scin >> p >> C[i], g[i]= {p - 1, i + 1};
 auto adj= g.adjacency_edge(1);
 auto dfs= [&](auto&& dfs, int v) -> PLC {
  PLC f;
  if (adj[v].size() == 0) {
   f.add_inf(), f.add_inf(true);
   return f;
  }
  for (int e: adj[v]) {
   int u= g[e].to(v);
   PLC fu= dfs(dfs, u);
   fu.shift(C[e]);
   fu.add_linear(1);
   fu.chmin_slide_win(-C[e], 0);
   fu.add_linear(-2);
   fu.chmin_cum();
   fu.add_linear(1);
   f+= fu;
  }
  return f;
 };
 scout << (long long)dfs(dfs, 0).min().value() << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "4 6\n1 5\n2 5\n2 8\n3 3\n3 2\n3 3\n2 9\n4 4\n4 3\n", "5\n"));
 return 0;
}