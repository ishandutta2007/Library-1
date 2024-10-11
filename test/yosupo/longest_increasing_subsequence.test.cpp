// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/longest_increasing_subsequence
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
 vector<int> A(N);
 for (int i= 0; i < N; ++i) cin >> A[i];
 auto [_, cand]= longest_increasing_subsequence(A);
 int K= cand.size();
 cout << K << '\n';
 for (int i= 0; i < K; ++i) cout << cand[i].front() << " \n"[i + 1 == K];
 return 0;
}
