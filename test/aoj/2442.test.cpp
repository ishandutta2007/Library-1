#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2442"
#define ERROR "0.00000001"
#include <iostream>
#include <iomanip>
#include <vector>
#include "src/Geometry/Point.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 int N;
 cin >> N;
 vector<Point<int>> ps(N);
 for (int i= 0; i < N; i++) cin >> ps[i];
 if (N & 1) cout << "NA" << '\n';
 else {
  Point p= ps[0] + ps[N / 2];
  bool isok= true;
  for (int i= N / 2; i--;) isok&= p == ps[i] + ps[N / 2 + i];
  if (isok) cout << p.x * 0.5 << " " << p.y * 0.5 << '\n';
  else cout << "NA" << '\n';
 }
 return 0;
}