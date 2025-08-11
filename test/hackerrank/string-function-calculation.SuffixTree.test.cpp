// competitive-verifier: PROBLEM https://www.hackerrank.com/challenges/string-function-calculation
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <string>
#include "src/String/SuffixTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string t;
 cin >> t;
 int N= t.length();
 SuffixArray sa(t);
 LCPArray lcp(sa);
 SuffixTree st(sa, lcp);
 long long ans= 0;
 for (auto [l, r, h, hh]: st) {
  ans= max(ans, (long long)(r - l) * hh);
 }
 cout << ans << '\n';
 return 0;
}