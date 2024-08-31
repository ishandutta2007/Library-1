// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1216
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include <array>
#include <tuple>

// 重みつき木

#include "src/DataStructure/KDTree.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
using namespace std;
struct RSQ {
 using T= int;
 static T ti() { return 0; }
 static T op(T l, T r) { return l + r; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 Graph g(N, N - 1);
 vector<long long> C(N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i] >> C[i], --g[i];
 HeavyLightDecomposition tree(g, 0);
 auto adj= g.adjacency_edge(0);
 vector<long long> dep(N);
 for (int i= 0, v; i < N; ++i)
  for (int e: adj[v= tree.to_vertex(i)])
   if (int u= g[e].to(v); u != tree.parent(v)) dep[u]= dep[v] + C[e];
 set<array<long long, 2>> st;
 vector<tuple<int, int, int, long long>> query;
 for (int i= 0; i < Q; ++i) {
  int tp, v;
  long long t, l;
  cin >> tp >> v >> t >> l, --v;
  if (tp == 0) {
   long long x= tree.to_seq(v), y= t + dep[v];
   query.emplace_back(1, 0, x, y);
   st.insert({x, y});
   auto path= tree.path(0, v);
   int u= -1;
   for (int i= path.size(); i--;) {
    auto [a, b]= path[i];
    if (dep[v] - dep[tree.to_vertex(a)] <= l) continue;
    for (++b; b - a > 1;) {
     int m= (a + b) / 2;
     (dep[v] - dep[tree.to_vertex(m)] > l ? a : b)= m;
    }
    u= tree.to_vertex(a);
    break;
   }
   if (u != -1) {
    x= tree.to_seq(u);
    query.emplace_back(-1, 0, x, y);
    st.insert({x, y});
   }
  } else {
   auto [l, r]= tree.subtree(v);
   query.emplace_back(0, l, r, t + dep[v]);
  }
 }
 KDTree<long long, 2, RSQ> kdt(st);
 for (auto [t, a, b, y]: query) {
  if (t == 0) cout << kdt.prod_cuboid(a, b - 1, 0, y) << '\n';
  else kdt.mul(b, y, t);
 }
 return 0;
}
