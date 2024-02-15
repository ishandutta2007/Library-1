#define PROBLEM "https://judge.yosupo.jp/problem/general_matching"
#include <iostream>
#include "src/Graph/general_matching.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 Graph g(N, M);
 for (int i= 0; i < M; ++i) cin >> g[i];
 auto ans= general_matching(g);
 cout << ans.size() << '\n';
 for (auto [u, v]: ans) cout << u << " " << v << '\n';
 return 0;
}