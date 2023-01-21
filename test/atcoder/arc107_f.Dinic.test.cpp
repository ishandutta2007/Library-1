#define PROBLEM "https://atcoder.jp/contests/arc107/tasks/arc107_f"
#include <iostream>
#include <vector>
#include "src/Optimization/MaxFlow.hpp"
#include "src/Optimization/monge_mincut.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, M;
 cin >> N >> M;
 vector<long long> A(N), B(N);
 for (int i= 0; i < N; i++) cin >> A[i];
 for (int i= 0; i < N; i++) cin >> B[i];
 vector<vector<bool>> adj(N, vector<bool>(N, false));
 for (int i= 0; i < M; i++) {
  int U, V;
  cin >> U >> V, U--, V--;
  adj[U][V]= adj[V][U]= true;
 }
 auto theta= [&](int i, int xi) {
  if (xi == 0) return B[i];
  if (xi == 2) return -B[i];
  return A[i];
 };
 const long long INF= 300 * 1e6 + 10;
 auto phi= [&](int i, int j, int xi, int xj) {
  if (adj[i][j] && xi + xj == 2 && xi != xj) return INF;
  return 0ll;
 };
 using MF= MaxFlow<Dinic<long long>>;
 auto [ans, x]= monge_mincut<MF>(N, 3, theta, phi);
 cout << -ans << '\n';
 return 0;
}
