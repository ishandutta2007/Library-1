// competitive-verifier: PROBLEM https://www.hackerrank.com/challenges/ashton-and-string
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <string>
#include "src/String/SuffixTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int t;
 cin >> t;
 while (t--) {
  string s;
  cin >> s;
  long long k;
  cin >> k, --k;
  SuffixArray sa(s);
  LCPArray lcp(sa);
  SuffixTree st(sa, lcp);
  for (auto [l, r, h, hh]: st) {
   long long sum= (long long)(hh - h) * (hh + h + 1) / 2;
   if (k >= sum) {
    k-= sum;
    continue;
   }
   for (int a= h + 1; a <= k; ++a) k-= a;
   cout << s[sa[l] + k] << '\n';
   break;
  }
 }
 return 0;
}