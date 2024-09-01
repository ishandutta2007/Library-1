// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc172/tasks/abc172_d
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// O(N^(3/4)/log N)
#include <iostream>
#include "src/NumberTheory/sum_on_primes.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long N;
 cin >> N;
 auto Ps= sums_of_powers_on_primes<long long>(N, 1);
 auto f= [](long long p, short e) {
  long long ret= e + 1;
  while (e--) ret*= p;
  return ret;
 };
 cout << multiplicative_sum(2 * Ps[1], f) << '\n';
 return 0;
}