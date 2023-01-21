#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/2/GRL_2_B"
#include <iostream>
#include "src/Graph/MinimumSpanningAborescense.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M, r;
 cin >> N >> M >> r;
 MinimumSpanningAborescense<int> graph(N);
 for (int i= 0; i < M; i++) {
  int s, t, w;
  cin >> s >> t >> w;
  graph.add_edge(s, t, w);
 }
 cout << graph.get_MSA(r).first << endl;
 return 0;
}