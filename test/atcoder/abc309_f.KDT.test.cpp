#define PROBLEM "https://atcoder.jp/contests/abc309/tasks/abc309_f"

// 2次元 + 走査
// 3次元より遅い

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
 vector<array<int, 3>> query;
 vector<array<int, 2>> xy;
 for (int i= 0; i < N; ++i) {
  int a[3];
  cin >> a[0] >> a[1] >> a[2];
  sort(a, a + 3);
  query.push_back({a[0], a[1], a[2]});
  xy.push_back({a[1], a[2]});
 }
 KDTree<int, 2, ROrQ> kdt(xy);
 sort(query.begin(), query.end(), [](auto a, auto b) { return a[0] == b[0] ? a[1] > b[1] : a[0] < b[0]; });
 bool isok= false;
 for (auto [h, w, d]: query) {
  isok= kdt.prod_cuboid(0, w - 1, 0, d - 1);
  if (isok) break;
  kdt.set(w, d, 1);
 }
 cout << (isok ? "Yes" : "No") << '\n';
 return 0;
}
