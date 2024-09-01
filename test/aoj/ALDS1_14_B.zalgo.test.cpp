// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/14/ALDS1_14_B
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <string>
#include "src/String/z_algorithm.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string T, P;
 cin >> T >> P;
 int n= P.size(), m= T.size();
 P+= '$' + T;
 auto Z= z_algorithm(P);
 for (int i= n + 1; i <= m + n; ++i)
  if (Z[i] == n) cout << i - n - 1 << '\n';
 return 0;
}