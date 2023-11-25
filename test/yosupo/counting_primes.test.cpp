#define PROBLEM "https://judge.yosupo.jp/problem/counting_primes"
#include <iostream>
#include "src/NumberTheory/ExtendedEratosthenesSieve.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 long long N;
 cin >> N;
 ExtendedEratosthenesSieve<long long> ees(N, 0);
 cout << ees.pwsum[0].sum() << '\n';
 return 0;
}