#define PROBLEM "https://atcoder.jp/contests/abc179/tasks/abc179_c"
// O(N^(2/3)log^(1/3)N))
#include <iostream>
#include "src/Math/prime_count.hpp"
#include "src/Math/multiplicative_and_additive.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace multiplicative_functions;
 long long N;
 cin >> N;
 cout << multiplicative_sum<long long>(N - 1, Divisor<long long, 0>::f, Divisor<long long, 0>::poly()) << '\n';
 return 0;
}