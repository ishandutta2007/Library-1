// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc279/tasks/abc279_d
// competitive-verifier: ERROR 0.00000001
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <iomanip>
#include <cmath>
#include "src/Optimization/fibonacci_search.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long A, B;
 cin >> A >> B;
 auto f= [&](long long n) { return (long double)B * n + A / sqrt(n + 1); };
 auto [x, fx]= fibonacci_search<MINIMIZE>(f, 0, 1e18);
 cout << fixed << setprecision(15) << fx << '\n';
 return 0;
}