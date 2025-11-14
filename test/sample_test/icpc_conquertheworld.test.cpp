// competitive-verifier: STANDALONE

// https://icpc.kattis.com/problems/conquertheworld
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
 int n;
 scin >> n;
 Graph g(n, n - 1);
 vector<int> c(n - 1);
 for (int i= 0; i < n - 1; ++i) scin >> g[i] >> c[i], --g[i];
 vector<int> x(n), y(n);
 for (int i= 0; i < n; ++i) scin >> x[i] >> y[i];
 using PLC= PiecewiseLinearConvex<long long>;
 auto adj= g.adjacency_edge(0);
 auto dfs= [&](auto&& dfs, int v, int p) -> PLC {
  PLC f;
  f.add_inf(true);
  for (int e: adj[v]) {
   int u= g[e].to(v);
   if (u == p) continue;
   PLC f_= dfs(dfs, u, v);
   f_.chmin_slide_win(-c[e], c[e]);
   f+= f_;
  }
  f.add_linear(y[v] - x[v]);
  return f;
 };
 scout << (long long)-dfs(dfs, 0, -1).min().value() << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main,
             "3\n"
             "1 2 5\n"
             "3 1 5\n"
             "2 1\n"
             "5 0\n"
             "1 3\n",
             "15\n"));
 assert(test(TEST::main,
             "6\n"
             "1 2 2\n"
             "1 3 5\n"
             "1 4 1\n"
             "2 5 5\n"
             "2 6 1\n"
             "0 0\n"
             "1 0\n"
             "2 1\n"
             "2 1\n"
             "0 1\n"
             "0 1\n",
             "9\n"));
 return 0;
}