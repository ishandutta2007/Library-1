#define PROBLEM "https://atcoder.jp/contests/abc268/tasks/abc268_e"
#include <iostream>
#include <algorithm>
#include "src/Optimization/fibonacci_search.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 int x[N];
 for (int i= 0; i < N; ++i) {
  int p;
  cin >> p;
  x[p]= i;
 }
 auto f= [&](int n) {
  int ret= 0;
  for (int i= 0; i < N; ++i) {
   int tmp= (x[i] + n - i + N) % N;
   ret+= min(tmp, N - tmp);
  }
  return ret;
 };
 auto [x1, fx1]= fibonacci_search<MINIMIZE>(f, 0, N / 2);
 auto [x2, fx2]= fibonacci_search<MINIMIZE>(f, N / 2, N);
 cout << min(fx1, fx2) << '\n';
 return 0;
}
