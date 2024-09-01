// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1625
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 128
// 区間を二次元の点に変換するテク maxクエリ

#include <iostream>
#include <map>
#include <array>
#include <vector>
#include <algorithm>
#include "src/DataStructure/SegmentTree_2D.hpp"
using namespace std;
struct RmaxQ {
 using T= long long;
 static T ti() { return 0; }
 static T op(T l, T r) { return max(l, r); }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n, q;
 cin >> n >> q;
 map<array<int, 2>, long long> mp;
 for (int i= 0; i < n; i++) {
  int a, b, c, d, e, f;
  cin >> a >> b >> c >> d >> e >> f;
  auto [l, r]= minmax({a, c, e});
  mp[{l, r}]= abs((long long)(c - a) * (f - b) - (long long)(d - b) * (e - a));
 }
 vector<tuple<int, int, int, long long>> query;
 for (int i= 0; i < q; i++) {
  int op;
  cin >> op;
  if (op == 1) {
   int a, b, c, d, e, f;
   cin >> a >> b >> c >> d >> e >> f;
   auto [l, r]= minmax({a, c, e});
   mp[{l, r}];
   query.emplace_back(op, l, r, abs((long long)(c - a) * (f - b) - (long long)(d - b) * (e - a)));
  } else {
   int l, r;
   cin >> l >> r;
   query.emplace_back(op, l, r, 0);
  }
 }
 SegmentTree_2D<int, RmaxQ> seg(mp);
 for (auto [op, l, r, x]: query) {
  if (op == 1) seg.mul(l, r, x);
  else {
   auto ans= seg.prod(l, r + 1, l, r + 1);
   cout << (ans ? ans : -1) << '\n';
  }
 }
 return 0;
}