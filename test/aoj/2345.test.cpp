#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2345"

// double型, log

#include <bits/stdc++.h>
#include "src/Math/SetPowerSeries.hpp"
#include "src/Graph/UndirectedGraphSetPowerSeries.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int N, M, P;
  cin >> N >> M >> P;
  UndirectedGraphSetPowerSeries g(N);
  for (int i = 0; i < M; i++) {
    int u, v;
    cin >> u >> v, g.add_edge(--u, --v);
  }
  if (P == 0) {
    cout << (g.connected_component_num().back() == 1) << '\n';
  } else {
    auto e = g.edge_space_rank();
    vector<double> h(e.size());
    for (int s = e.size(); s--;) h[s] = pow(100. / P, e[s]);
    auto f = SetPowerSeries<>::log(h);
    cout << fixed << setprecision(12)
         << f.back() * pow(double(P) / 100, e.back()) << '\n';
  }
  return 0;
}