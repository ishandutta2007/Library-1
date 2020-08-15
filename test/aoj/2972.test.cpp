#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2972"

#define ERROR "0.0001"

//焼きなまし法（実は凸関数だから三分探索や山登りでいいらしい）
//点の距離しか使わないから幾何ライブラリは大げさ

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/!geometry_temp.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  int N, K;
  cin >> N >> K;
  vector<Point> ps(N);
  for (int i = 0; i < N; i++) cin >> ps[i];
  random_device seed_gen;
  mt19937 rng(seed_gen());
  using urd = uniform_real_distribution<>;
  auto score = [&](Point p) {
    vector<Real> dis(N);
    for (int i = 0; i < N; i++) dis[i] = dist(ps[i], p);
    sort(dis.rbegin(), dis.rend());
    Real res = 0;
    for (int i = 0; i < K; i++) res += dis[i];
    return res;
  };
  auto sim_ann = [&](double st, double ed, double delta) {
    Point p = {urd(-1000, 1000)(rng), urd(-1000, 1000)(rng)};
    Real res = score(p);
    for (double tmp = st; tmp >= ed; tmp *= delta) {
      Point np
          = p + Point({urd(-1000, 1000)(rng), urd(-1000, 1000)(rng)}) * tmp;
      Real nw = score(np);
      if (nw < res || urd(0, 1)(rng) <= exp((res - nw) / tmp)) p = np, res = nw;
    }
    return res;
  };
  cout << fixed << setprecision(12) << sim_ann(1, 1e-10, .999) << endl;
  return 0;
}