// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc354/tasks/abc354_f
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Misc/longest_increasing_subsequence.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int T;
 cin >> T;
 while (T--) {
  int N;
  cin >> N;
  vector<int> A(N);
  for (int i= 0; i < N; ++i) cin >> A[i];
  auto [idx, _]= longest_increasing_subsequence(A);
  vector<int> ans;
  for (int i= 0; i < N; ++i)
   if (idx[i] != -1) ans.push_back(i);
  int m= ans.size();
  cout << m << '\n';
  for (int i= 0; i < m; ++i) cout << ans[i] + 1 << " \n"[i + 1 == m];
 }
 return 0;
}
