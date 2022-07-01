#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2647"
#include <bits/stdc++.h>
#include "src/Graph/MinimumSpanningAborescense.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, M;
  cin >> N >> M;
  MinimumSpanningAborescense<int> graph(N);
  bool able[N];
  fill(able, able + N, true);
  for (int i = 0; i < M; i++) {
    int a, b;
    cin >> a >> b;
    able[b] = false;
    graph.add_edge(a, b, 0);
    graph.add_edge(b, a, 1);
  }
  int min_cost = INT_MAX;
  vector<int> capitals;
  for (int i = 0; i < N; i++)
    if (able[i]) {
      int cost = graph.get_MSA(i).first;
      if (cost < min_cost) {
        min_cost = cost;
        capitals = {i};
      } else if (cost == min_cost) {
        capitals.push_back(i);
      }
    }
  cout << capitals.size() << " " << min_cost << endl;
  for (int i = 0; i < capitals.size(); i++)
    cout << (i ? " " : "") << capitals[i];
  cout << endl;
  return 0;
}