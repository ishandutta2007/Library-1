#define PROBLEM "https://judge.yosupo.jp/problem/bipartitematching"
#include <iostream>
#include "src/Graph/Graph.hpp"
#include "src/Graph/general_matching.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int L, R, M;
 cin >> L >> R >> M;
 Graph g(L + R, M);
 for (int i= 0; i < M; ++i) cin >> g[i], g[i].second+= L;
 auto ans= general_matching(g);
 cout << ans.size() << '\n';
 for (auto [l, r]: ans) cout << l << " " << r - L << '\n';
 return 0;
}