#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2442"

#define ERROR "0.00000001"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/_geometry_temp.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  int N;
  cin >> N;
  vector<Point> ps(N);
  for (int i = 0; i < N; i++) cin >> ps[i];
  if (N & 1)
    cout << "NA" << endl;
  else {
    Point ans = (ps[0] + ps[N / 2]) / 2;
    bool isok = true;
    for (int i = 0; i < N / 2; i++) isok &= ans == (ps[i] + ps[N / 2 + i]) / 2;
    if (isok)
      cout << ans << endl;
    else
      cout << "NA" << endl;
  }
  return 0;
}