// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc324/tasks/abc324_g
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include <array>
#include "src/DataStructure/WaveletMatrix.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 vector<int> A(N);
 for (int i= 0; i < N; ++i) cin >> A[i], --A[i];
 WaveletMatrix wm(A);
 vector<array<int, 4>> dat;
 dat.push_back({0, N, 0, N});
 int Q;
 cin >> Q;
 while (Q--) {
  int t, s, x;
  cin >> t >> s >> x;
  auto [l, r, u, d]= dat[s];
  if (t == 1) {
   if (wm.count(l, r, u, d) < x) dat.push_back({0, 0, 0, 0});
   else {
    int ok= r, ng= l - 1;
    while (ok - ng > 1) {
     int m= (ok + ng) / 2;
     if (wm.count(l, m, u, d) >= x) ok= m;
     else ng= m;
    }
    dat[s]= {l, ok, u, d}, dat.push_back({ok, r, u, d});
   }
  } else {
   if (--x; d <= x) dat.push_back({0, 0, 0, 0});
   else if (x < u) dat[s]= {0, 0, 0, 0}, dat.push_back({l, r, u, d});
   else dat[s]= {l, r, u, x + 1}, dat.push_back({l, r, x + 1, d});
  }
  {
   auto [l, r, u, d]= dat.back();
   cout << wm.count(l, r, u, d) << '\n';
  }
 }
 return 0;
}