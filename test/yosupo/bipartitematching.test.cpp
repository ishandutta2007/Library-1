#define PROBLEM "https://judge.yosupo.jp/problem/bipartitematching"
#include <iostream>
#include "src/Graph/BipartiteMatching.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int L, R, M;
 cin >> L >> R >> M;
 BipartiteMatching bm(L, R);
 while (M--) {
  int a, b;
  cin >> a >> b;
  bm.add_edge(a, b);
 }
 bm.build();
 auto ans= bm.max_matching();
 cout << ans.size() << '\n';
 for (auto [l, r]: ans) cout << l << " " << r << '\n';
 return 0;
}