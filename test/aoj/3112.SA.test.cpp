#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/3112"
#include <iostream>
#include <vector>
#include <string>
#include "src/String/SuffixArray.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string S, T;
 cin >> S >> T;
 int k;
 cin >> k;
 int n= S.length();
 S+= "$" + T;
 SuffixArray sa(S);
 LCPArray lcp(sa);
 int N= S.length();
 vector<int> len(n);
 for (int k= 0, j= -1; k < N; ++k) {
  if (int i= sa[k]; i < n) {
   if (j != -1) len[i]= max(len[i], lcp(i, j));
  } else if (i > n) j= i;
 }
 for (int k= N, j= -1; k--;) {
  if (int i= sa[k]; i < n) {
   if (j != -1) len[i]= max(len[i], lcp(i, j));
  } else if (i > n) j= i;
 }
 vector<int> dp(n + 1);
 dp[0]= 1, dp[1]= -1;
 for (int i= 0; i < n; dp[i + 1]+= dp[i], ++i) {
  if (!dp[i]) continue;
  if (len[i] < k) continue;
  ++dp[i + k];
  if (i + len[i] + 1 <= n) --dp[i + len[i] + 1];
 }
 cout << (dp[n] ? "Yes" : "No") << '\n';
 return 0;
}