#define PROBLEM "https://atcoder.jp/contests/abc172/tasks/abc172_d"
// O(N^(3/4)/log N)
#include <iostream>
#include "src/NumberTheory/ExtendedEratosthenesSieve.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 auto f= [](long long p, short e) {
  long long ret= e + 1;
  while (e--) ret*= p;
  return ret;
 };
 long long N;
 cin >> N;
 ExtendedEratosthenesSieve<long long> ees(N, 1);
 cout << ees.multiplicative_sum(f, {0, 2}) << '\n';
 return 0;
}