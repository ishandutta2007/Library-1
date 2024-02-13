#define PROBLEM "https://yukicoder.me/problems/no/1976"
#include <iostream>
#include <array>
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
 using Dat= array<int, 2>;
 using Data= array<int, 3>;
 auto put_edge= [&](int, int, const Dat& d) { return Data{d[0] + 1, 0, d[1]}; };
 auto op= [&](const Data& l, const Data& r) {
  int v[]= {l[0], l[1], r[0], r[1]};
  sort(v, v + 4);
  return Data{v[3], v[2], max(l[2], r[2])};
 };
 auto put_vertex= [&](int, const Data& d) { return Dat{d[0], max(d[2], d[0] + d[1])}; };
 Rerooting<Dat> dp(g, put_edge, op, Data{0, 0, 0}, put_vertex);
 int ans= 1 << 30;
 for (auto [u, v]: g) {
  int X= dp(u, v)[1], Y= dp(v, u)[1];
  ans= min(ans, max({X, Y, (X + 1) / 2 + (Y + 1) / 2 + 1}));
 }
 cout << ans << '\n';
 return 0;
}