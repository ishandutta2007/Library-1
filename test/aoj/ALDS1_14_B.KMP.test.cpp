#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/14/ALDS1_14_B"
#include <iostream>
#include <string>
#include "src/String/KnuthMorrisPratt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string T, P;
 cin >> T >> P;
 KnuthMorrisPratt KMP(P);
 auto ans= KMP.pattern_match(T);
 for (auto a: ans) {
  cout << a << "\n";
 }
 cout << '\n';
 return 0;
}