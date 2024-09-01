// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/longest_common_substring
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 128
#include <iostream>
#include <string>
#include "src/String/SuffixArray.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string S, T;
 cin >> S >> T;
 int n= S.length();
 S+= "$" + T;
 int N= S.length();
 SuffixArray sa(S);
 LCPArray lcp(sa);
 int a= 0, c= 0, len= 0;
 for (int i= 0; i + 1 < N; ++i) {
  int x= sa[i], y= sa[i + 1];
  if (x > y) swap(x, y);
  if (x < n && n < y && len < lcp[i]) {
   len= lcp[i];
   a= x, c= y - n - 1;
  }
 }
 cout << a << " " << a + len << " " << c << " " << c + len << "\n";
 return 0;
}