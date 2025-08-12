// competitive-verifier: PROBLEM https://www2.ioi-jp.org/camp/2019/2019-sp-tasks/day1/examination.pdf
// competitive-verifier: TLE 2.5
// competitive-verifier: MLE 64

#include <iostream>
#include <array>
#include <map>
#include "src/DataStructure/KDTree.hpp"
using namespace std;
struct RSQ {
 using T= int;
 static T ti() { return 0; }
 static T op(T l, T r) { return l + r; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, Q;
 cin >> N >> Q;
 map<array<int, 3>, int> mp;
 for (int i= 0; i < N; ++i) {
  int S, T;
  cin >> S >> T;
  mp[{S, T, S + T}]+= 1;
 }
 KDTree<int, 3, RSQ> kdt(mp);
 static constexpr int INF= 0x7fffffff;
 for (int i= 0; i < Q; ++i) {
  int X, Y, Z;
  cin >> X >> Y >> Z;
  cout << kdt.prod_cuboid(X, INF, Y, INF, Z, INF) << '\n';
 }
 return 0;
}