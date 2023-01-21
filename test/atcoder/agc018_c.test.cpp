#define PROBLEM "https://atcoder.jp/contests/agc018/tasks/agc018_c"
#include <iostream>
#include <algorithm>
#include "src/Optimization/min_Lconvex.hpp"
using namespace std;
// O(MAX_A log log MAX_A)
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int x[3], n= 0;
 for (int j= 0; j < 3; j++) cin >> x[j], n+= x[j];
 int c[n][3];
 for (int i= 0; i < n; i++)
  for (int j= 0; j < 3; j++) cin >> c[i][j];
 auto f= [&](const std::vector<int> &q) {
  long long ret= 0;
  for (int i= 0; i < n; i++) ret+= max({c[i][0] - q[0], c[i][1] - q[1], c[i][2] - q[2]});
  for (int j= 0; j < 3; j++) ret+= 1LL * x[j] * q[j];
  return ret;
 };
 cout << min_Lconvex<int, long long>(f, {0, 0, 0}, 1 << 29).first << '\n';
 return 0;
}