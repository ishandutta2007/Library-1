// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2647
// competitive-verifier: TLE 1
#include <iostream>
#include <algorithm>
#include <vector>
#include "src/Graph/Graph.hpp"
#include "src/Graph/minimum_spanning_aborescence.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 Graph g(N);
 vector<int> c, able(N, true);
 for (int i= 0; i < M; i++) {
  int a, b;
  cin >> a >> b;
  able[b]= false;
  g.add_edge(a, b), c.push_back(0);
  g.add_edge(b, a), c.push_back(1);
 }
 int best= 1 << 30;
 vector<int> capitals;
 for (int i= 0; i < N; i++)
  if (able[i]) {
   auto [cost, _]= minimum_spanning_aborescence(g, c, i);
   if (best > cost) best= cost, capitals= {i};
   else if (cost == best) capitals.push_back(i);
  }
 int n= capitals.size();
 cout << n << " " << best << '\n';
 for (int i= 0; i < n; ++i) cout << capitals[i] << " \n"[i == n - 1];
 return 0;
}