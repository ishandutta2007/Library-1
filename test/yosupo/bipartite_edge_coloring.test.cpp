#define PROBLEM "https://judge.yosupo.jp/problem/bipartite_edge_coloring"
#include <iostream>
#include <algorithm>
#include "src/Graph/BipartiteGraph.hpp"
#include "src/Graph/bipartite_edge_coloring.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int L, R, M;
 cin >> L >> R >> M;
 BipartiteGraph bg(L, R, M);
 for (int i= 0; i < M; ++i) cin >> bg[i], bg[i].second+= L;
 auto ans= bipartite_edge_coloring(bg);
 cout << *max_element(ans.begin(), ans.end()) + 1 << '\n';
 for (auto a: ans) cout << a << '\n';
 return 0;
}