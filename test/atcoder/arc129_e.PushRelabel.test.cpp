#define PROBLEM "https://atcoder.jp/contests/arc129/tasks/arc129_e"

#include <bits/stdc++.h>
#include "src/Algorithm/PushRelabel.hpp"
#include "src/Graph/MaxFlow.hpp"
#include "src/Algorithm/monge_mincut.hpp"
using namespace std;

// 解無しの判定のverify

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using MF = MaxFlow<PushRelabel<long long>>;
  int N, M;
  cin >> N >> M;
  vector<vector<long long>> A(N, vector<long long>(M)),
      C(N, vector<long long>(M));
  for (int i = 0; i < N; i++)
    for (int k = 0; k < M; k++) cin >> A[i][k] >> C[i][k];
  vector<vector<long long>> W(N, vector<long long>(N));
  for (int i = 0; i < N; i++)
    for (int j = i + 1; j < N; j++) cin >> W[i][j];
  auto theta = [&](int i, int k) { return C[i][k]; };
  auto phi = [&](int i, int j, int k, int l) {
    return abs(A[i][k] - A[j][l]) * W[i][j];
  };
  auto [ans, x] = monge_mincut<long long, MF>(N, M, theta, phi);
  cout << ans << '\n';
  return 0;
}