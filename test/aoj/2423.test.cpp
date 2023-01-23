#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2423"
#include <iostream>
#include <vector>
#include "src/Geometry/!geometry_temp.hpp"
#include "src/Geometry/circle_functions.hpp"
#include "src/Graph/MatchingBipartite.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geometry;
 int n, m;
 cin >> n >> m;
 MatchingBipartite graph(m, n);
 Real rs[n];
 for (int i= 0; i < n; i++) cin >> rs[i];
 for (int j= 0; j < m; j++) {
  int p;
  cin >> p;
  vector<Point> ps(p);
  for (int k= 0; k < p; k++) cin >> ps[k];
  Real r= min_enclosing_circle(ps).r;
  for (int i= 0; i < n; i++)
   if (sgn(rs[i] - r) >= 0) graph.add_edge(j, i);
 }
 auto ans= graph.lexicographically_matching();
 if (ans.first < m) cout << "NG" << endl;
 else
  for (int a: ans.second.first) cout << a + 1 << endl;
 return 0;
}