#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2647"
#include <iostream>
#include <algorithm>
#include <vector>
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
 for (int i= 0; i < M; i++) {
  int a, b;
  cin >> a >> b;
  able[b]= false;
  graph.add_edge(a, b, 0), graph.add_edge(b, a, 1);
 }
 int min_cost= 1 << 30;
 vector<int> capitals;
 for (int i= 0; i < N; i++)
  if (able[i]) {
   int cost= graph.get_MSA(i).first;
   if (cost < min_cost) {
    min_cost= cost, capitals= {i};
   } else if (cost == min_cost) {
    capitals.push_back(i);
   }
  }
 int n= capitals.size();
 cout << n << " " << min_cost << '\n';
 for (int i= 0; i < n; ++i) cout << capitals[i] << " \n"[i == n - 1];
 return 0;
}