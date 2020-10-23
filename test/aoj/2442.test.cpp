#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2442"
#define ERROR "0.00000001"
#include <bits/stdc++.h>
#include "src/Geometry/!geometry_temp.hpp"
using namespace std;

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
      cout << fixed << setprecision(12) << ans << endl;
    else
      cout << "NA" << endl;
  }
  return 0;
}