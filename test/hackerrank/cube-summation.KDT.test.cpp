#define PROBLEM "https://www.hackerrank.com/challenges/cube-summation"
#include <iostream>
#include <map>
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
 using KDT= KDTree<3, int, RSQ>;
 int T;
 cin >> T;
 while (T--) {
  int n, m;
  cin >> n >> m;
  map<array<int, 3>, long long> mp;
  vector<array<int, 7>> query;
  while (m--) {
   string op;
   cin >> op;
   if (op[0] == 'U') {
    int x, y, z, w;
    cin >> x >> y >> z >> w;
    mp[{x, y, z}];
    query.push_back({0, x, y, z, w, 0, 0});
   } else {
    int x1, y1, z1, x2, y2, z2;
    cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
    query.push_back({1, x1, y1, z1, x2, y2, z2});
   }
  }
  KDT kdt({mp.begin(), mp.end()});
  for (auto q: query) {
   if (q[0] == 0) {
    auto [_, x, y, z, w, __, ___]= q;
    kdt.set(w, {x, y, z});
   } else {
    auto [_, x1, y1, z1, x2, y2, z2]= q;
    cout << kdt.fold({x1, x2}, {y1, y2}, {z1, z2}) << '\n';
   }
  }
 }
 return 0;
}