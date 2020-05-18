#define PROBLEM "https://yukicoder.me/problems/no/417"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Graph/Tree_DP.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, M;
  cin >> N >> M;
  using Vll = vector<long long>;
  using Pill = pair<int, long long>;
  auto f = [&](const Vll &vdp, const Vll &chdp) {
    Vll ret(M + 1, 0);
    for (int i = 0; i <= M; i++) {
      for (int j = 0; i + j <= M; j++) {
        ret[i + j] = max(ret[i + j], vdp[i] + chdp[j]);
      }
    }
    return ret;
  };
  auto g = [&](const Vll &dp, const Pill &dat) {
    Vll ret(M + 1);
    for (int i = 0; i <= M; i++) {
      ret[i] = i >= 2 * dat.first ? dp[i - 2 * dat.first] + dat.second : 0;
    }
    return ret;
  };
  Tree_DP<Vll, Pill> graph(N, f, Vll(M, 0), g);
  long long U[N];
  for (int i = 0; i < N; i++) {
    cin >> U[i];
  }
  for (int i = 0; i < N - 1; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    graph.add_edge(a, b, {c, U[b]});
    graph.add_edge(b, a, {c, U[a]});
  }
  auto res = graph.run(0);
  long long ans = res[M] + U[0];
  cout << ans << endl;
  return 0;
}