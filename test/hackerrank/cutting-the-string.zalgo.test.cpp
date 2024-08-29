// competitive-verifier: PROBLEM https://www.hackerrank.com/contests/101hack50/challenges/cutting-the-string
// competitive-verifier: TLE 0.5
// c++14 までだった

#include <iostream>
#include <string>
#include "src/String/z_algorithm.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string s;
 cin >> s;
 int N= s.length();
 long long ans= 0;
 for (int i= 0; i < N; ++i) {
  auto z= z_algorithm(s.substr(i));
  auto p= z_to_kmp(z);
  int n= p.size();
  for (int j= 0; j < n; ++j) p[j]= j - p[j];
  for (int j= 1; j < n; ++j) {
   if (j % p[j] == 0 && j >= p[j] * 2) ans+= (j / p[j] - 1) * 2;
   ans+= 1;
  }
 }
 cout << ans << '\n';
 return 0;
}