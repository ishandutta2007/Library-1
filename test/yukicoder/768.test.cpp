#define PROBLEM "https://yukicoder.me/problems/no/768"

//全方位木DP

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Graph/Tree_DP.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N;
  cin >> N;
  auto f
      = [](const short &vdp, const short &chdp) { return short(vdp & chdp); };
  auto g = [](const short &dp, const int &dat) { return short(!dp); };
  Tree_DP<short> graph(N, f, 1, g);
  for (int i = 0; i < N - 1; i++) {
    int a, b;
    cin >> a >> b;
    a--, b--;
    graph.add_edge(a, b);
    graph.add_edge(b, a);
  }
  auto ret = graph.run_rerooting();
  vector<int> ans;
  for (int i = 0; i < N; i++)
    if (ret[i]) ans.push_back(i + 1);
  cout << ans.size() << endl;
  for (int &a : ans) {
    cout << a << endl;
  }
  return 0;
}