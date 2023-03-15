#define PROBLEM "https://judge.yosupo.jp/problem/bipartitematching"
#include <iostream>
#include "src/Graph/Matching.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int L, R, M;
 cin >> L >> R >> M;
 Matching graph(L + R);
 while (M--) {
  int a, b;
  cin >> a >> b;
  graph.add_edge(a, L + b);
 }
 graph.build();
 cout << graph.max_matching().size() << '\n';
 for (int l= 0; l < L; ++l)
  if (int r= graph.match(l); r != -1) cout << l << " " << r - L << '\n';
 return 0;
}