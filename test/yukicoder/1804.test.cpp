// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1804
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
 vector<int> P(N);
 for (int i= 0; i < N; ++i) cin >> P[i];
 auto [_, cand]= longest_increasing_subsequence(P);
 vector<int> ans;
 for (auto &c: cand)
  if (c.size() == 1) ans.push_back(P[c[0]]);
 int m= ans.size();
 cout << m << '\n';
 for (int i= 0; i < m; ++i) cout << ans[i] << " \n"[i + 1 == m];
 if (!m) cout << " \n";
 return 0;
}