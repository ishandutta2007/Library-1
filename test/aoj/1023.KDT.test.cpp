// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/1023
// competitive-verifier: TLE 0.5

// 円形クエリ

#include <iostream>
#include <vector>
#include <array>
#include "src/DataStructure/KDTree.hpp"
using namespace std;
struct RSQ {
 using T= int;
 static T ti() { return 0; }
 static T op(T l, T r) { return l + r; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 for (int AN, BN, R; cin >> AN >> BN >> R && AN;) {
  vector<array<int, 2>> A(AN), B(BN);
  for (int i= 0; i < AN; ++i) cin >> A[i][0] >> A[i][1];
  for (int i= 0; i < BN; ++i) cin >> B[i][0] >> B[i][1];
  KDTree<int, 2, RSQ> kdt(B, 1);
  long long ans= 0;
  for (auto [x, y]: A) ans+= kdt.prod_ball(x, y, R * 4);
  cout << ans << '\n';
 }
 return 0;
}