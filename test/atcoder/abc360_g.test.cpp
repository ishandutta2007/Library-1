// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc360/tasks/abc360_g
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Misc/longest_increasing_subsequence.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N;
 cin >> N;
 vector<int> A(N + 2);
 A[0]= -(1 << 30), A[N + 1]= 1 << 30;
 for (int i= 1; i <= N; ++i) cin >> A[i];
 auto [_, cand]= longest_increasing_subsequence(A);
 int K= cand.size();
 for (int i= 0; i + 1 < K; ++i) {
  int n= cand[i].size(), m= cand[i + 1].size();
  for (int j= 0, k= 0; j < n; ++j) {
   while (k < m && cand[i][j] + 2 > cand[i + 1][k]) ++k;
   if (k < m && A[cand[i][j]] + 2 <= A[cand[i + 1][k]]) return cout << K - 1 << '\n', 0;
  }
 }
 cout << K - 2 << '\n';
 return 0;
}
