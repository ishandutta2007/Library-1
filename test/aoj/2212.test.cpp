#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2212"
#include <bits/stdc++.h>
#include "src/String/AhoCorasick.hpp"
using namespace std;

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  int di[4] = {1, 0, -1, 0};
  int dj[4] = {0, 1, 0, -1};
  char dc[4] = {'D', 'R', 'U', 'L'};
  constexpr int INF = INT_MAX / 10;
  for (int N, M; cin >> N >> M && N;) {
    auto ij2n = [M](int i, int j) { return i * M + j; };
    auto n2ij = [M](int n) { return make_pair(n / M, n % M); };
    auto sn2m = [N, M](int s, int n) { return s * N * M + n; };
    auto m2sn = [N, M](int m) { return make_pair(m / (N * M), m % (N * M)); };
    string maze[N];
    int S, G;
    for (int i = 0; i < N; i++) {
      cin >> maze[i];
      for (int j = 0; j < M; j++) {
        if (maze[i][j] == 'S') S = ij2n(i, j);
        if (maze[i][j] == 'G') G = ij2n(i, j);
      }
    }
    int P;
    cin >> P;
    vector<string> t(P);
    for (int i = 0; i < P; i++) cin >> t[i];
    AhoCorasick<char> ac(t);
    queue<int> que;
    int K = N * M * ac.state_size();
    int dist[K];
    fill_n(dist, K, INF);
    int m0 = sn2m(ac.initial_state(), S);
    dist[m0] = 0;
    que.push(m0);
    int ans = -1;
    while (!que.empty()) {
      int m = que.front();
      que.pop();
      auto [s, n] = m2sn(m);
      if (n == G) {
        ans = dist[m];
        break;
      }
      auto [i, j] = n2ij(n);
      for (int d = 0; d < 4; d++) {
        int ni = i + di[d], nj = j + dj[d];
        if (ni < 0 || N <= ni || nj < 0 || M <= nj) continue;
        if (maze[ni][nj] == '#') continue;
        int ns = ac.transition(s, dc[d]);
        if (ac.is_accept(ns)) continue;
        int nn = ij2n(ni, nj);
        int nm = sn2m(ns, nn);
        if (dist[nm] != INF) continue;
        dist[nm] = dist[m] + 1;
        que.push(nm);
      }
    }
    cout << ans << '\n';
  }
  return 0;
}