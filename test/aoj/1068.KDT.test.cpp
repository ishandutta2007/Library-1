#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/UOA/UAPC/1068"
#include <iostream>
#include <vector>
#include "src/DataStructure/KDTree.hpp"

// 典型2Dセグ木 minクエリ
using namespace std;
struct RminQ {
 using T= int;
 static T ti() { return 0x7fffffff; }
 static T op(T l, T r) { return min(l, r); }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using KDT= KDTree<2, int, RminQ>;
 int r, c, q;
 for (; cin >> r >> c >> q && r != 0;) {
  vector<typename KDT::PosVal> v;
  for (int i= 0; i < r; i++)
   for (int j= 0; j < c; j++) {
    int grid;
    cin >> grid;
    v.emplace_back(array{i, j}, grid);
   }
  KDT kdt(v);
  while (q--) {
   int r1, c1, r2, c2;
   cin >> r1 >> c1 >> r2 >> c2;
   cout << kdt.fold({r1, r2}, {c1, c2}) << '\n' << flush;
  }
 }
 return 0;
}