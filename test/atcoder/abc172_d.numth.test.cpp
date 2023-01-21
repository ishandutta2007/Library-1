#define PROBLEM "https://atcoder.jp/contests/abc172/tasks/abc172_d"
// O(N)
#include <iostream>
#include "src/Math/Sieve.hpp"
#include "src/Math/multiplicative_and_additive.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace multiplicative_functions;
 int N;
 cin >> N;
 auto d= Sieve<>::multiplicative_table<long long>(N, Divisor<int, 0>::f);
 long long ans= 0;
 for (long long i= 1; i <= N; i++) ans+= d[i] * i;
 cout << ans << endl;
 return 0;
}