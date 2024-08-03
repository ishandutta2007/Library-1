// competitive-verifier: PROBLEM https://www.hackerrank.com/challenges/cube-summation
#include <iostream>
#include <set>
#include <vector>
#include <array>
#include "src/DataStructure/KDTree.hpp"
// 3次元
using namespace std;
struct RSQ {
 using T= long long;
 static T ti() { return 0; }
 static T op(T l, T r) { return l + r; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int T;
 cin >> T;
 while (T--) {
  int n, m;
  cin >> n >> m;
  set<array<int, 3>> xyz;
  vector<array<int, 7>> query;
  while (m--) {
   string op;
   cin >> op;
   if (op[0] == 'U') {
    int x, y, z, w;
    cin >> x >> y >> z >> w;
    xyz.insert({x, y, z});
    query.push_back({0, x, y, z, w, 0, 0});
   } else {
    int x1, y1, z1, x2, y2, z2;
    cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
    query.push_back({1, x1, y1, z1, x2, y2, z2});
   }
  }
  KDTree<int, 3, RSQ> kdt(xyz);
  for (auto q: query) {
   if (q[0] == 0) {
    auto [_, x, y, z, w, __, ___]= q;
    kdt.set(x, y, z, w);
   } else {
    auto [_, x1, y1, z1, x2, y2, z2]= q;
    cout << kdt.prod_cuboid(x1, x2, y1, y2, z1, z2) << '\n';
   }
  }
 }
 return 0;
}