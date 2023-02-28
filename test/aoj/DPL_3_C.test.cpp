#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/3/DPL_3_C"
#include <iostream>
#include <vector>
#include <algorithm>
#include <src/Misc/CartesianTree.hpp>
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 vector<long long> h(N);
 for (int i= 0; i < N; ++i) cin >> h[i];
 CartesianTree ct(h);
 long long ans= 0;
 for (int i= 0; i < N; ++i) {
  auto [l, r]= ct.range(i);
  ans= max(ans, h[i] * (r - l));
 }
 cout << ans << '\n';
 return 0;
}