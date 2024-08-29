// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/sum_of_totient_function
// competitive-verifier: TLE 0.5
// O(N^(3/4)/log N)
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/sum_on_primes.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 long long N;
 cin >> N;
 auto Ps= sums_of_powers_on_primes<Mint>(N, 1);
 auto f= [](long long p, short e) { return Mint(p).pow(e - 1) * (p - 1); };
 cout << multiplicative_sum(Ps[1] - Ps[0], f) << '\n';
 return 0;
}