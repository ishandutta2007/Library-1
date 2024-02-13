#define PROBLEM "https://yukicoder.me/problems/no/1295"
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Graph g(N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i], --g[i];
 g.build(N, 0);
 vector<int> mx(N), mx2(N), mn(N);
 for (int i= 0; i < N; ++i) {
  vector<int> vs;
  for (auto e: g(i)) vs.push_back(g[e] - i);
  sort(vs.begin(), vs.end());
  int n= vs.size();
  mx[i]= vs[n - 1], mn[i]= vs[0];
  mx2[i]= n > 1 ? vs[n - 2] : -1;
 }
 auto put_edge= [&](int v, int e, int d) {
  int u= g[e] - v;
  if (d >= 4 || (d == 3 && v != mx[u])) return 4;
  if (d == 0 && v == mn[u]) return 0;
  if (u == mx[v] || u == mn[v]) return 2;
  if (u == mx2[v]) return 3;
  return 4;
 };
 auto op= [&](int l, int r) { return l + r; };
 auto put_vertex= [&](int v, int d) { return d; };
 for (auto x: Rerooting<int>(g, put_edge, op, 0, put_vertex)) cout << (x <= 2 ? "Yes" : "No") << '\n';
 return 0;
}