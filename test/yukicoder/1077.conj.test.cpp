// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1077
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 PiecewiseLinearConvex<int> f;
 f.add_inf(), f.add_inf(true);
 for (int i= 0; i < N; ++i) {
  int Y;
  cin >> Y;
  f.add_inf(true);
  f.add_linear(-Y);
  f.chmin_slide_win(-1, 1);
  f.add_linear(Y);
 }
 cout << -f(0) << '\n';
 return 0;
}