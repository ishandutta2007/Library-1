// competitive-verifier: PROBLEM https://www.hackerrank.com/contests/university-codesprint-5/challenges/cube-loving-numbers
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/NumberTheory/tables.hpp"
// メビウス関数
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int T;
 cin >> T;
 auto mu= mobius_table(1'000'010);
 while (T--) {
  long long N, ans= 0;
  cin >> N;
  for (long long x= 2; x * x * x <= N; x++) ans-= N / (x * x * x) * mu[x];
  cout << ans << '\n';
 }
 return 0;
}