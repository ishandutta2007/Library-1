#define PROBLEM "https://www.hackerrank.com/contests/university-codesprint-5/challenges/cube-loving-numbers"
#include <iostream>
#include "src/NumberTheory/famous_arithmetic_functions.hpp"
#include "src/NumberTheory/Sieve.hpp"
// メビウス関数
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace famous_arithmetic_functions;
 int T;
 cin >> T;
 auto mu= Sieve<>::multiplicative_table<long long>(1'000'010, mul::Moebius<long long>::f);
 while (T--) {
  long long N, ans= 0;
  cin >> N;
  for (long long x= 2; x * x * x <= N; x++) ans-= N / (x * x * x) * mu[x];
  cout << ans << '\n';
 }
 return 0;
}