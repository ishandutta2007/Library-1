// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc087/tasks/arc085_E
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/arc085/tasks/arc085_c
#include <iostream>
#include "src/Optimization/MaxFlow.hpp"
#include "src/Optimization/monge_mincut.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N;
 cin >> N;
 vector<long long> a(N);
 for (int i= 0; i < N; i++) cin >> a[i];
 auto theta= [&](int i, int xi) { return xi ? 0 : -a[i]; };
 const long long INF= 1e12 + 10;
 auto phi= [&](int i, int j, int xi, int xj) {
  if (xi == 1 && xj == 0 && (j + 1) % (i + 1) == 0) return INF;
  if (xj == 1 && xi == 0 && (i + 1) % (j + 1) == 0) return INF;
  return 0ll;
 };
 using MF= MaxFlow<PushRelabel<long long>>;
 auto [ans, x]= monge_mincut<MF>(N, 2, theta, phi);
 cout << -ans << '\n';
 return 0;
}
