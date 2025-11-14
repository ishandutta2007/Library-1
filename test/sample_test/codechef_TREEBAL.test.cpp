// competitive-verifier: STANDALONE

// https://www.codechef.com/problems/TREEBAL
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
 using PLC= PiecewiseLinearConvex<long long, true, 1 << 24>;
 int T;
 scin >> T;
 while (T--) {
  int N;
  scin >> N;
  Graph g(N, N - 1);
  vector<int> C(N - 1), D(N - 1);
  for (int i= 0; i < N - 1; ++i) scin >> g[i] >> C[i] >> D[i], --g[i];
  auto adj= g.adjacency_edge(0);
  vector<PLC> fs(N);
  auto dfs= [&](auto&& dfs, int v, int p) -> void {
   if (adj[v].size() == (p != -1)) {
    fs[v].add_inf(), fs[v].add_inf(true);
    return;
   }
   for (int e: adj[v]) {
    int u= g[e].to(v);
    if (u == p) continue;
    dfs(dfs, u, v);
    PLC f= fs[u];
    f.shift(C[e]);
    f.add_linear(-D[e]);
    f.chmin_cum();
    f.add_linear(D[e] * 2);
    f.chmin_cum(true);
    f.add_linear(-D[e]);
    if (PLC::pool_empty()) PLC::rebuild(fs);
    fs[v]+= f;
   }
   if (PLC::pool_empty()) PLC::rebuild(fs);
  };
  dfs(dfs, 0, -1);
  scout << (long long)fs[0].min().value() << '\n';
  vector<long long> ans(N - 1);
  auto dfs2= [&](auto&& dfs2, int v, int p, long long x) -> void {
   for (int e: adj[v]) {
    int u= g[e].to(v);
    if (u == p) continue;
    PLC f= fs[u];
    f.add_abs(D[e], x - C[e]);
    auto [l, r]= f.argmin();
    ans[e]= x - l;
    dfs2(dfs2, u, v, l);
   }
   if (PLC::pool_empty()) PLC::rebuild(fs);
  };
  auto [l, r]= fs[0].argmin();
  dfs2(dfs2, 0, -1, l);
  for (int i= 0; i < N - 1; ++i) scout << ans[i] << '\n';
  PLC::reset();
 }
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "1\n5\n1 2 5 4\n1 3 15 15\n2 4 3 2\n2 5 5 1\n", "19\n5\n15\n10\n10\n"));
 return 0;
}