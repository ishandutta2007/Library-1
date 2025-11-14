// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc134/tasks/abc134_e
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Misc/longest_increasing_subsequence.hpp"
// 広義単調
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N;
 cin >> N;
 vector<int> A(N);
 for (int i= 0; i < N; ++i) cin >> A[N - i - 1];
 auto [_, cand]= longest_increasing_subsequence(A, false);
 cout << cand.size() << '\n';
 return 0;
}
