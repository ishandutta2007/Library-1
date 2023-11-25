#define PROBLEM "https://atcoder.jp/contests/abc179/tasks/abc179_c"
// O(N^(3/4)/logN))
#include <iostream>
#include "src/NumberTheory/ExtendedEratosthenesSieve.hpp"
#include "src/NumberTheory/famous_arithmetic_functions.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace famous_arithmetic_functions;
 using D= mul::Divisor<long long, 0>;
 long long N;
 cin >> N;
 ExtendedEratosthenesSieve<long long> ees(N - 1, 0);
 cout << ees.multiplicative_sum(D::f, D::poly()) << '\n';
 return 0;
}