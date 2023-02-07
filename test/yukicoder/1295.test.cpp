#define PROBLEM "https://yukicoder.me/problems/no/1295"
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include "src/Graph/rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Tree tree(N);
 for (int i= 0; i < N - 1; ++i) {
  int a, b;
  cin >> a >> b;
  tree.add_edge(--a, --b);
 }
 tree.build();
 vector<int> ma(N), ma2(N), mi(N);
 for (int i= 0; i < N; ++i) {
  vector<int> vs;
  for (auto e: tree[i]) vs.push_back(e.to);
  sort(vs.begin(), vs.end());
  int n= vs.size();
  ma[i]= vs[n - 1], mi[i]= vs[0];
  ma2[i]= n > 1 ? vs[n - 2] : -1;
 }
 auto f_ee= [&](int l, int r) { return l + r; };
 auto f_ve= [&](int d, int p, auto e) {
  if (d >= 4 || (d == 3 && p != ma[e.to])) return 4;
  if (d == 0 && p == mi[e.to]) return 0;
  if (e.to == ma[p] || e.to == mi[p]) return 2;
  if (e.to == ma2[p]) return 3;
  return 4;
 };
 auto f_ev= [&](int d, int) { return d; };
 auto dp= rerooting<int>(tree, f_ee, f_ve, f_ev, 0);
 for (auto x: dp) cout << (x <= 2 ? "Yes" : "No") << '\n';
 return 0;
}