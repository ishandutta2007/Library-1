#define PROBLEM "https://yukicoder.me/problems/no/763"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Graph/Tree_DP.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N;
  cin >> N;
  using Pii = pair<int, int>;
  auto f = [](const Pii &vdp, const Pii &chdp) {
    Pii ret = vdp;
    ret.first += max(chdp.first - 1, chdp.second);
    ret.second += max(chdp.first, chdp.second);
    return ret;
  };
  Tree_DP<Pii> graph(N, f, {1, 0});
  for (int i = 0; i < N - 1; i++) {
    int u, v;
    cin >> u >> v;
    u--, v--;
    graph.add_edge(u, v);
    graph.add_edge(v, u);
  }
  auto ans = graph.run(0);
  cout << max(ans.first, ans.second) << endl;
  return 0;
}