// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc179/tasks/abc179_c
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// O(N^(3/4)/logN))
#include <iostream>
#include "src/NumberTheory/sum_on_primes.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long N;
 cin >> N;
 auto Ps= sums_of_powers_on_primes<long long>(N - 1, 0);
 auto f= [](long long, short e) { return e + 1; };
 cout << multiplicative_sum(2 * Ps[0], f) << '\n';
 return 0;
}