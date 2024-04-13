#define PROBLEM "https://atcoder.jp/contests/abc309/tasks/abc309_f"

// 3次元
// めっちゃ高速に通る

#include <iostream>
#include <vector>
#include <array>
#include "src/DataStructure/KDTree.hpp"
using namespace std;
struct ROrQ {
 using T= bool;
 static T ti() { return 0; }
 static T op(T l, T r) { return l | r; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 vector<array<int, 3>> xyz;
 for (int i= 0; i < N; ++i) {
  int a[3];
  cin >> a[0] >> a[1] >> a[2];
  sort(a, a + 3);
  xyz.push_back({a[0], a[1], a[2]});
 }
 KDTree<int, 3, ROrQ> kdt(xyz, 1);
 bool isok= false;
 for (auto [h, w, d]: xyz) {
  isok= kdt.prod_cuboid(0, h - 1, 0, w - 1, 0, d - 1);
  if (isok) break;
 }
 cout << (isok ? "Yes" : "No") << '\n';
 return 0;
}