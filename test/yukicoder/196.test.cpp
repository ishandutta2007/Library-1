#define PROBLEM "https://yukicoder.me/problems/no/196"

//二乗の木DP

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/ModInt.hpp"
#include "Graph/Tree_DP.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<int(1e9 + 7)>;
  int N, K;
  cin >> N >> K;
  auto f = [](const vector<Mint> &vdp, const vector<Mint> &chdp) {
    vector<Mint> ret(vdp.size() + chdp.size() - 1, 0);
    for (int i = 0; i < vdp.size() - 1; i++) {
      for (int j = 0; j < chdp.size(); j++) {
        ret[i + j] += vdp[i] * chdp[j];
      }
    }
    ret.back() = 1;
    return ret;
  };
  Tree_DP<vector<Mint>> graph(N, f, {1, 1});
  for (int i = 0; i < N - 1; i++) {
    int a, b;
    cin >> a >> b;
    graph.add_edge(a, b);
    graph.add_edge(b, a);
  }
  auto ans = graph.run(0);
  cout << ans[K] << endl;
  return 0;
}