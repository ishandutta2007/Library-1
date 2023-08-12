#define PROBLEM "https://yukicoder.me/problems/no/180"
#include <iostream>
#include <vector>
#include "src/Optimization/fibonacci_search.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 long long a[N], b[N];
 for (int i= 0; i < N; ++i) cin >> a[i] >> b[i];
 auto f= [&](long long n) {
  long long mx= 0, mi= 1e18;
  for (int i= 0; i < N; ++i) {
   long long tmp= a[i] + b[i] * n;
   mx= max(mx, tmp);
   mi= min(mi, tmp);
  }
  return (mx - mi) * 2 + n;
 };
 auto [x, fx]= fibonacci_search<MINIMIZE>(f, 1, 1e9);
 cout << x << '\n';
 return 0;
}
