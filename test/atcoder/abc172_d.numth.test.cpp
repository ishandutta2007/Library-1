#define PROBLEM "https://atcoder.jp/contests/abc172/tasks/abc172_d"
// O(N)
#include <iostream>
#include "src/NumberTheory/tables.hpp"
#include "src/NumberTheory/famous_arithmetic_functions.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace famous_arithmetic_functions;
 int N;
 cin >> N;
 auto d= multiplicative_table<long long>(N, mul::Divisor<int, 0>::f);
 long long ans= 0;
 for (long long i= 1; i <= N; i++) ans+= d[i] * i;
 cout << ans << '\n';
 return 0;
}