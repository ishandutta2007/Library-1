#define PROBLEM "https://yukicoder.me/problems/no/1077"
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvexfunction.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 PiecewiseLinearConvexfunction f;
 for (int i= 0; i < N; ++i) {
  int Y;
  cin >> Y;
  f.cumulative_chmin();
  f.add_abs(1, Y);
 }
 cout << f.min() << '\n';
 return 0;
}