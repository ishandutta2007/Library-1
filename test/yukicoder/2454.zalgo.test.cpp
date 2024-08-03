// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2454
#include <iostream>
#include <string>
#include "src/String/z_algorithm.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int T;
 cin >> T;
 while (T--) {
  int N;
  string S;
  cin >> N >> S;
  auto Z= z_algorithm(S);
  int ans= 0;
  for (int i= 1; i < N; ++i) {
   if (i < Z[i]) ++ans;
   else if (i == Z[i] && i < N - i) ++ans;
   else if (S[Z[i]] < S[i + Z[i]]) ++ans;
  }
  cout << ans << '\n';
 }
 return 0;
}