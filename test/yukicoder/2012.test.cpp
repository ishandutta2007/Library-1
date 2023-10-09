#define PROBLEM "https://yukicoder.me/problems/no/2012"
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
 ConvexHullTrick_XY<long long> cht;
 int x[N], y[N];
 for (int i= 0; i < N; ++i) cin >> x[i] >> y[i], cht.insert_line(x[i], -y[i]);
 long long ans= 0;
 for (int i= N; i--;) ans= max(ans, cht.get_max(y[i], x[i]));
 cout << ans << '\n';
 return 0;
}