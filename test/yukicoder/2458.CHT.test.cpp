#define PROBLEM "https://yukicoder.me/problems/no/2458"
#include <iostream>
#include <array>
#include <algorithm>
#include "src/Optimization/ConvexHullTrick.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 ConvexHullTrick<long long, MAXIMIZE> cht;
 long long ans= -1e18;
 cht.insert(0, 0);
 for (int i= 0;;) {
  long long Q;
  cin >> Q;
  long long dp= cht.query(Q);
  ans= max(ans, dp);
  if (++i == N) break;
  cht.insert(Q, dp);
 }
 cout << ans << '\n';
 return 0;
}