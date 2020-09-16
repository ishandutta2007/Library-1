#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Prelim/0366"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Graph/StronglyConnectedComponents.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, M;
  cin >> N >> M;
  StronglyConnectedComponents graph(N);
  vector<pair<int, int>> edges(M);
  for (int i = 0; i < M; i++) {
    int s, t;
    cin >> s >> t;
    edges[i] = {s, t};
    graph.add_edge(s, t);
  }
  auto scc = graph.get_SCC();
  auto I = scc.second;
  int n = scc.first.size();
  if (n == 1) {
    cout << 0 << endl;
  } else {
    bool src[n], dst[n];
    fill(src, src + n, true);
    fill(dst, dst + n, true);
    for (auto &e : edges) {
      if (I[e.first] == I[e.second]) continue;
      src[I[e.second]] = false;
      dst[I[e.first]] = false;
    }
    int src_num = 0, dst_num = 0;
    for (int i = 0; i < n; i++) {
      src_num += src[i];
      dst_num += dst[i];
    }
    cout << max(src_num, dst_num) << endl;
  }
  return 0;
}