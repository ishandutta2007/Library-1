// competitive-verifier: PROBLEM https://atcoder.jp/contests/s8pc_2/tasks/s8pc_2_e
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/s8pc-2/tasks/s8pc_2_e
#include <iostream>
#include <string>
#include "src/String/SuffixArray.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string S;
 cin >> S;
 int N= S.length();
 SuffixArray sa(S);
 LCPArray lcp(sa);
 long long ans= (long long)N * (N + 1) / 2 * (N + 2) / 3;
 for (int i= 0; i < N - 1; i++) ans-= (long long)lcp[i] * (lcp[i] + 1) / 2;
 cout << ans << '\n';
 return 0;
}