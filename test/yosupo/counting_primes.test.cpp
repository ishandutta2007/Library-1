// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/counting_primes
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/NumberTheory/sum_on_primes.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 long long N;
 cin >> N;
 auto Ps= sums_of_powers_on_primes<long long>(N, 0);
 cout << Ps[0].sum() << '\n';
 return 0;
}