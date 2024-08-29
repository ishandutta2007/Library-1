// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/sort_points_by_argument
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Geometry/angle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace geo;
 int N;
 cin >> N;
 vector<Point<long long>> ps(N);
 for (int i= 0; i < N; ++i) cin >> ps[i];
 sort(ps.begin(), ps.end(), AngleComp<long long>());
 for (auto &p: ps) cout << p.x << " " << p.y << "\n";
 return 0;
}