#define PROBLEM "https://judge.yosupo.jp/problem/directedmst"
#include <iostream>
#include <vector>
#include <numeric>
#include "src/Graph/MinimumSpanningAborescense.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M, r;
 cin >> N >> M >> r;
 MinimumSpanningAborescense<long long> graph(N);
 for (int i= 0; i < M; i++) {
  int s, t, w;
  cin >> s >> t >> w;
  graph.add_edge(s, t, w);
 }
 auto ans= graph.get_MSA(r);
 vector<int> par(N);
 iota(par.begin(), par.end(), 0);
 for (auto e: ans.second) par[e.dst]= e.src;
 cout << ans.first << '\n';
 for (int i= 0; i < N; i++) cout << par[i] << " \n"[i == N - 1];
 return 0;
}