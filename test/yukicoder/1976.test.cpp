#define PROBLEM "https://yukicoder.me/problems/no/1976"
#include <iostream>
#include <array>
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
  int u, v;
  cin >> u >> v;
  tree.add_edge(--u, --v);
 }
 tree.build();
 using Dat= array<int, 2>;
 using Data= array<int, 3>;
 auto f_ee= [&](const Data& l, const Data& r) {
  int v[]= {l[0], l[1], r[0], r[1]};
  sort(v, v + 4);
  return Data{v[3], v[2], max(l[2], r[2])};
 };
 auto f_ve= [&](const Dat& d, int, auto) { return Data{d[0] + 1, 0, d[1]}; };
 auto f_ev= [&](const Data& d, int) { return Dat{d[0], max(d[2], d[0] + d[1])}; };
 auto dp= rerooting<Dat>(tree, f_ee, f_ve, f_ev, Data{0, 0, 0});
 int ans= 1 << 30;
 for (int i= 1; i < N; ++i) {
  int u= i, v= tree.parent(i);
  int X= dp.get(u, v)[1], Y= dp.get(v, u)[1];
  ans= min(ans, max({X, Y, (X + 1) / 2 + (Y + 1) / 2 + 1}));
 }
 cout << ans << '\n';
 return 0;
}