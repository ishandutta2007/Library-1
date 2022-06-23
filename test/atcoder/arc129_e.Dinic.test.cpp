#define PROBLEM "https://atcoder.jp/contests/arc129/tasks/arc129_e"

#include <bits/stdc++.h>
#include "src/Optimization/Dinic.hpp"
#include "src/Optimization/MaxFlow.hpp"
#include "src/Optimization/monge_mincut.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using MF = MaxFlow<Dinic<long long>>;
  int N, M;
  cin >> N >> M;
  vector<vector<long long>> A(N, vector<long long>(M)),
      C(N, vector<long long>(M));
  for (int i = 0; i < N; i++)
    for (int k = 0; k < M; k++) cin >> A[i][k] >> C[i][k];
  vector<vector<long long>> W(N, vector<long long>(N));
  for (int i = 0; i < N; i++)
    for (int j = i + 1; j < N; j++) cin >> W[i][j];
  auto theta = [&](int i, int xi) { return C[i][xi]; };
  auto phi = [&](int i, int j, int xi, int xj) {
    return abs(A[i][xi] - A[j][xj]) * W[i][j];
  };
  auto [ans, x] = monge_mincut<MF>(N, M, theta, phi);
  cout << ans << '\n';
  return 0;
}