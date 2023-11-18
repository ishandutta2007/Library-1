#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/14/ALDS1_14_D"
#include <iostream>
#include <string>
#include "src/String/SuffixArray.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string t;
 cin >> t;
 SuffixArray sa(t);
 int Q;
 cin >> Q;
 while (Q--) {
  string p;
  cin >> p;
  auto [l, r]= sa.pattern_matching(p);
  cout << (r > l) << '\n';
 }
 cout << flush;
 return 0;
}