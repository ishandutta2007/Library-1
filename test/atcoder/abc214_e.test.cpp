// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc214/tasks/abc214_e
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <algorithm>
#include <numeric>
#include "src/DataStructure/RangeSet.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int T;
 cin >> T;
 while (T--) {
  int N;
  cin >> N;
  int L[N], R[N];
  for (int i= 0; i < N; ++i) cin >> L[i] >> R[i];
  int ord[N];
  iota(ord, ord + N, 0), sort(ord, ord + N, [&](int l, int r) { return R[l] < R[r]; });
  RangeSet<int> rs;
  bool isok= true;
  for (int i: ord) {
   int l= rs.mex(L[i]);
   isok&= l <= R[i];
   rs.insert(l);
  }
  cout << (isok ? "Yes" : "No") << '\n';
 }
 return 0;
}