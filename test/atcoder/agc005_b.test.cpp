// competitive-verifier: PROBLEM https://atcoder.jp/contests/agc005/tasks/agc005_b
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Misc/CartesianTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N;
 cin >> N;
 vector<int> A(N);
 for (int i= 0; i < N; ++i) cin >> A[i];
 CartesianTree ct(A);
 long long ans= 0;
 for (int i= N; i--;) {
  auto [l, r]= ct.range(i);
  ans+= (long long)(r - i) * (i - l + 1) * A[i];
 }
 cout << ans << '\n';
 return 0;
}
