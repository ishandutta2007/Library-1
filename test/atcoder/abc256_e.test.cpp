#define PROBLEM "https://atcoder.jp/contests/abc256/tasks/abc256_e"
#include <iostream>
#include <vector>
#include "src/Graph/Graph.hpp"
#include "src/Graph/StronglyConnectedComponents.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Graph g(N, N);
 for (int i= 0, X; i < N; ++i) cin >> X, g[i]= {i, --X};
 vector<int> C(N);
 for (int i= 0; i < N; ++i) cin >> C[i];
 StronglyConnectedComponents scc(g);
 long long ans= 0;
 for (int i= scc.size(); i--;) {
  auto blk= scc.block(i);
  if (blk.size() == 1) continue;
  int mn= 1 << 30;
  for (int v: blk) mn= min(mn, C[v]);
  ans+= mn;
 }
 cout << ans << '\n';
 return 0;
}