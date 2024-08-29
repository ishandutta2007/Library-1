// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/bipartitematching
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/Graph/BipartiteGraph.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int L, R, M;
 cin >> L >> R >> M;
 BipartiteGraph bg(L, R, M);
 for (int i= 0; i < M; ++i) cin >> bg[i], bg[i].second+= L;
 auto [ans, _]= bipartite_matching(bg);
 cout << ans.size() << '\n';
 for (int e: ans) {
  auto [l, r]= bg[e];
  cout << l << " " << r - L << '\n';
 }
 return 0;
}