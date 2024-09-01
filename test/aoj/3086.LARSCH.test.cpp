// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/3086
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/DataStructure/SegmentTree.hpp"
#include "src/Optimization/simplified_larsch_dp.hpp"
using namespace std;
struct RMQ {
 using T= long long;
 static T ti() { return 1e18; }
 static T op(T l, T r) { return min(l, r); }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, L;
 cin >> N >> L;
 vector<long long> a(N);
 for (int i= 0; i < N; ++i) cin >> a[i], a[i]= -a[i];
 SegmentTree<RMQ> seg(a);
 auto w= [&](int i, int j) -> long long {
  if (i - j < L) return 1e18;
  return seg.fold(j, i);
 };
 cout << -simplified_larsch_dp(N, w)[N] << '\n';
 return 0;
}