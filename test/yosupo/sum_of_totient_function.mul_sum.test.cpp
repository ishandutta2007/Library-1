#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_totient_function"
// O(N^(3/4)/log N)
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/ExtendedEratosthenesSieve.hpp"
#include "src/NumberTheory/famous_arithmetic_functions.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 using namespace famous_arithmetic_functions;
 using T= mul::Totient<Mint>;
 long long N;
 cin >> N;
 ExtendedEratosthenesSieve<Mint> ees(N, 1);
 cout << ees.multiplicative_sum(T::f, T::poly()) << '\n';
 return 0;
}