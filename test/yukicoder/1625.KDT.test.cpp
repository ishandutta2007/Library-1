#define PROBLEM "https://yukicoder.me/problems/no/1625"
#include <iostream>
#include <map>
#include <array>
#include <vector>
#include <algorithm>
#include "src/DataStructure/KDTree.hpp"
// 区間を二次元の点に変換するテク maxクエリ
using namespace std;
struct RmaxQ {
 using T= long long;
 static T ti() { return 0; }
 static T op(T l, T r) { return max(l, r); }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using KDT= KDTree<2, long long, RmaxQ>;
 int n, q;
 cin >> n >> q;
 map<array<long long, 2>, long long> mp;
 for (int i= 0; i < n; i++) {
  long long a, b, c, d, e, f;
  cin >> a >> b >> c >> d >> e >> f;
  auto [l, r]= minmax({a, c, e});
  mp[{l, r}]= abs((c - a) * (f - b) - (d - b) * (e - a));
 }
 vector<array<long long, 4>> query;
 for (int i= 0; i < q; i++) {
  long long op;
  cin >> op;
  if (op == 1) {
   long long a, b, c, d, e, f;
   cin >> a >> b >> c >> d >> e >> f;
   auto [l, r]= minmax({a, c, e});
   mp[{l, r}];
   query.push_back({op, l, r, abs((c - a) * (f - b) - (d - b) * (e - a))});
  } else {
   long long l, r;
   cin >> l >> r;
   query.push_back({op, l, r});
  }
 }
 KDT kdt({mp.begin(), mp.end()});
 for (auto [op, l, r, x]: query) {
  if (op == 1) {
   kdt.set(kdt.get(l, r) + x, l, r);
  } else {
   auto ans= kdt.fold({l, r}, {l, r});
   cout << (ans ? ans : -1) << '\n';
  }
 }
 return 0;
}