// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc213/tasks/abc213_f
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// ロリハでSA構築はTLE

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include "src/String/SuffixArray.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 string S;
 cin >> S;
 SuffixArray sa(S);
 LCPArray lcp(sa);
 vector<long long> B(N), C(N);
 stack<pair<long long, int>> stb, stc;
 for (int i= 1; i < N; ++i) {
  int cnt= 1;
  B[i]= B[i - 1];
  while (stb.size()) {
   auto [h, c]= stb.top();
   if (h < lcp[i - 1]) break;
   stb.pop();
   cnt+= c;
   B[i]-= h * c;
  }
  B[i]+= (long long)lcp[i - 1] * cnt;
  stb.emplace(lcp[i - 1], cnt);
 }
 for (int i= N; --i;) {
  int cnt= 1;
  C[i - 1]= C[i];
  while (stc.size()) {
   auto [h, c]= stc.top();
   if (h < lcp[i - 1]) break;
   stc.pop();
   cnt+= c;
   C[i - 1]-= h * c;
  }
  C[i - 1]+= (long long)lcp[i - 1] * cnt;
  stc.emplace(lcp[i - 1], cnt);
 }
 vector<long long> ans(N);
 for (int i= 0; i < N; ++i) {
  int j= sa[i];
  ans[j]= B[i] + C[i] + N - j;
 }
 for (int i= 0; i < N; ++i) cout << ans[i] << '\n';
 return 0;
}