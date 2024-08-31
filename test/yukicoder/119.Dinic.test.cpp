// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/119
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Optimization/MaxFlow.hpp"
#include "src/Optimization/monge_mincut.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N;
 cin >> N;
 vector<long long> B(N), C(N);
 for (int i= 0; i < N; i++) cin >> B[i] >> C[i];
 int M;
 cin >> M;
 vector<vector<bool>> to(N, vector<bool>(N, false));
 for (int i= 0; i < M; i++) {
  int D, E;
  cin >> D >> E;
  to[D][E]= true;
 }
 const long long INF= 1ll << 62;
 auto theta= [&](int i, int xi) {
  if (xi == 0) return -C[i];
  if (xi == 1) return 0ll;
  return -B[i];
 };
 auto phi= [&](int i, int j, int xi, int xj) {
  if (to[i][j] && xi == 2 && xj == 0) return INF;
  return 0ll;
 };
 using MF= MaxFlow<Dinic<long long>>;
 auto [ans, x]= monge_mincut<MF>(N, 3, theta, phi);
 cout << -ans << '\n';
 return 0;
}
