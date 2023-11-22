#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2863"
#include <iostream>
#include <algorithm>
#include <string>
#include "src/String/AhoCorasick.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
int main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 int N;
 cin >> N;
 vector<string> S(N);
 for (int i= 0; i < N; i++) cin >> S[i];
 AhoCorasick ac(S);
 string t;
 cin >> t;
 int M= t.length();
 Mint dp[M + 1];
 fill_n(dp, M + 1, 0);
 dp[0]= 1;
 for (int i= 0, s= ac.initial_state();;) {
  auto ps= ac.matched_patterns(s);
  for (int j: ps) {
   int k= i - S[j].length();
   if (i < 0) continue;
   dp[i]+= dp[k];
  }
  if (i == M) break;
  s= ac.transition(s, t[i++]);
 }
 cout << dp[M] << '\n';
 return 0;
}