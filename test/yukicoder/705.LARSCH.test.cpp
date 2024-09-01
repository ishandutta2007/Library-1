// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/705
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <algorithm>
#include "src/Optimization/simplified_larsch_dp.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n;
 cin >> n;
 long long a[n], x[n], y[n];
 for (int i= 0; i < n; ++i) cin >> a[i];
 for (int i= 0; i < n; ++i) cin >> x[i];
 for (int i= 0; i < n; ++i) cin >> y[i];
 auto f= [](long long x) { return abs(x * x * x); };
 auto w= [&](int i, int j) { return f(x[j] - a[i - 1]) + f(y[j]); };
 cout << simplified_larsch_dp(n, w)[n] << '\n';
 return 0;
}