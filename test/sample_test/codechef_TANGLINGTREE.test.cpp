// competitive-verifier: STANDALONE

// https://www.codechef.com/problems/TANGLINGTREE
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
 using PLC= PiecewiseLinearConvex<int>;
 int T;
 scin >> T;
 while (T--) {
  int N;
  scin >> N;
  vector<int> W(N);
  for (int i= 0; i < N; ++i) scin >> W[i];
  Graph g(N, N - 1);
  vector<int> R(N - 1);
  for (int e= 0; e < N - 1; ++e) scin >> g[e] >> R[e], --g[e];
  auto adj= g.adjacency_edge(0);
  auto dfs= [&](auto&& dfs, int v, int p) -> pair<PLC, PLC> {
   PLC f1, f2;
   for (int e: adj[v]) {
    int u= g[e].to(v);
    if (u == p) continue;
    auto [g1, g2]= dfs(dfs, u, v);
    g1.chmin_cum();
    g1.shift(-R[e]);
    g2.chmin_cum(true);
    g2.shift(R[e]);
    f1+= g2;
    f2+= g1;
   }
   f1.add_linear(-W[v]);
   f2.add_linear(W[v]);
   f1.add_inf();
   f2.add_inf(true);
   if (PLC::pool_empty()) PLC::rebuild(f1, f2);
   return {f1, f2};
  };
  auto [f, _]= dfs(dfs, 0, -1);
  scout << -f.min().value() << '\n';
  PLC::reset();
 }
 return 0;
}
}
signed main() {
 assert(test(TEST::main,
             "3\n"
             "3\n"
             "6 9 4\n"
             "1 2 2\n"
             "2 3 1\n"
             "6\n"
             "1 2 2 6 100 100\n"
             "1 2 9\n"
             "2 3 17\n"
             "2 4 3\n"
             "3 5 1\n"
             "3 6 4\n"
             "3\n"
             "120734269 1000000000 1\n"
             "1 2 300000000\n"
             "2 3 300000000\n",
             "16\n"
             "527\n"
             "300000000000000000\n"));
 return 0;
}