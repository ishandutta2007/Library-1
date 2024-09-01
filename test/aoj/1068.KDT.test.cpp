// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/UOA/UAPC/1068
// competitive-verifier: TLE 1.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <array>
#include "src/DataStructure/KDTree.hpp"

// minクエリ
using namespace std;
struct RminQ {
 using T= int;
 static T ti() { return 0x7fffffff; }
 static T op(T l, T r) { return min(l, r); }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 for (int r, c, q; cin >> r >> c >> q && r;) {
  vector<array<int, 3>> xy;
  for (int i= 0; i < r; i++)
   for (int j= 0; j < c; j++) {
    int grid;
    cin >> grid;
    xy.push_back({i, j, grid});
   }
  KDTree<int, 2, RminQ> kdt(xy);
  while (q--) {
   int r1, c1, r2, c2;
   cin >> r1 >> c1 >> r2 >> c2;
   cout << kdt.prod_cuboid(r1, r2, c1, c2) << '\n';
  }
 }
 return 0;
}