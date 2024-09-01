// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1718
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, K;
 cin >> N >> K;
 Graph g(N, N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i], --g[i];
 vector<int> cnt(N);
 for (int i= 0, D; i < K; ++i) cin >> D, ++cnt[--D];
 using Data= array<int, 2>;
 auto put_edge= [&](int v, int e, Data d) {
  if (cnt[g[e].to(v)] || d[0]) d[0]+= 2, d[1]+= 1;
  return d;
 };
 auto op= [&](const Data& l, const Data& r) { return Data{l[0] + r[0], max(l[1], r[1])}; };
 auto put_vertex= [&](int, const Data& d) { return d; };
 for (auto [a, b]: Rerooting<Data>(g, put_edge, op, Data{0, 0}, put_vertex)) cout << a - b << '\n';
 return 0;
}