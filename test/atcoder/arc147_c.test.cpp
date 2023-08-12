#define PROBLEM "https://atcoder.jp/contests/arc147/tasks/arc147_c"
#include <iostream>
#include <algorithm>
#include "src/Optimization/fibonacci_search.hpp"
using namespace std;
namespace arc147c {
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 int L[N], R[N];
 int mn= 1e9, mx= 0;
 for (int i= 0; i < N; ++i) cin >> L[i] >> R[i], mn= min(mn, L[i]), mx= max(mx, R[i]);
 auto f= [&](int t) {
  long long x[N];
  for (int i= 0; i < N; ++i) x[i]= clamp(t, L[i], R[i]);
  sort(x, x + N);
  long long ret= 0, now= 0;
  for (int i= 1; i < N; ++i) now+= (x[i] - x[i - 1]) * i, ret+= now;
  return ret;
 };
 auto [x, fx]= fibonacci_search<MINIMIZE>(f, mn, mx);
 cout << fx << '\n';
 return 0;
}
}