// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/point_add_rectangle_sum
// competitive-verifier: TLE 1
#include <iostream>
#include <map>
#include <array>
#include <vector>
#include "src/DataStructure/KDTree.hpp"
using namespace std;
struct RSQ {
 using T= long long;
 static T ti() { return 0; }
 static T op(T l, T r) { return l + r; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, Q;
 cin >> N >> Q;
 map<array<int, 2>, long long> mp;
 vector<array<int, 4>> query;
 for (int i= 0; i < N; i++) {
  int x, y, w;
  cin >> x >> y >> w;
  mp[{x, y}]+= w;
 }
 for (int i= 0; i < Q; i++) {
  int op;
  cin >> op;
  if (op) {
   int l, d, r, u;
   cin >> l >> d >> r >> u;
   query.push_back({l, d, r, u});
  } else {
   int x, y, w;
   cin >> x >> y >> w;
   query.push_back({-1, x, y, w});
   mp[{x, y}];
  }
 }
 KDTree<long long, 2, RSQ> kdt(mp);
 for (int i= 0; i < Q; i++) {
  if (query[i][0] != -1) {
   auto [l, d, r, u]= query[i];
   cout << kdt.prod_cuboid(l, r - 1, d, u - 1) << '\n';
  } else {
   auto [_, x, y, w]= query[i];
   kdt.mul(x, y, w);
  }
 }
 return 0;
}