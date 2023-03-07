#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2423"
#include <iostream>
#include <vector>
#include "src/Geometry/!geometry_temp.hpp"
#include "src/Geometry/circle_functions.hpp"
#include "src/Graph/BipartiteMatching.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geometry;
 int n, m;
 cin >> n >> m;
 BipartiteMatching bm(m, n);
 Real rs[n];
 for (int i= 0; i < n; i++) cin >> rs[i];
 for (int j= 0; j < m; j++) {
  int p;
  cin >> p;
  vector<Point> ps(p);
  for (int k= 0; k < p; k++) cin >> ps[k];
  Real r= min_enclosing_circle(ps).r;
  for (int i= 0; i < n; i++)
   if (sgn(rs[i] - r) >= 0) bm.add_edge(j, i);
 }
 bm.build<1>();
 if (bm.max_matching().size() < m) cout << "NG" << '\n';
 else
  for (int l= 0; l < m; ++l) cout << bm.l_to_r(l) + 1 << '\n';
 return 0;
}