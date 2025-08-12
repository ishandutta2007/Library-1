// competitive-verifier: PROBLEM https://www2.ioi-jp.org/camp/2010/2010-sp-tasks/2010-sp-day2_21.pdf#2
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include "src/String/AhoCorasick.hpp"
using namespace std;
int main() {
 ios::sync_with_stdio(0);
 cin.tie(0);
 int N;
 cin >> N;
 string S;
 cin >> S;
 int M= S.length();
 vector<string> t(N);
 for (int i= 0; i < N; i++) cin >> t[i];
 AhoCorasick ac(t);
 vector<int> dp(M + 1, 1 << 30);
 dp[0]= 0;
 for (int i= 0, s= ac.initial_state();;) {
  auto ps= ac.matched_patterns(s);
  for (int j: ps) {
   int l= t[j].length();
   if (l == i) dp[i]= 1;
   for (int k= i - 1; k >= 0 && k > i - l; k--) {
    dp[i]= min(dp[i], dp[k] + 1);
   }
  }
  if (i == M) break;
  s= ac.transition(s, S[i++]);
 }
 cout << dp[M] << '\n';
 return 0;
}