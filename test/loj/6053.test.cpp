// competitive-verifier: PROBLEM https://loj.ac/p/6053
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/sum_on_primes.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<1000000007>;
 long long n;
 cin >> n;
 if (n == 1) return cout << 1 << '\n', 0;
 auto Ps= sums_of_powers_on_primes<Mint>(n, 1);
 auto x= Ps[1] - Ps[0];
 x.add(2, 2);
 auto f= [](long long p, int c) { return p ^ c; };
 cout << multiplicative_sum<Mint>(x, f) << '\n';
 return 0;
}