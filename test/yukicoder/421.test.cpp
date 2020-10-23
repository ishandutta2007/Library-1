#define PROBLEM "https://yukicoder.me/problems/no/421"
#include <bits/stdc++.h>
#include "src/Graph/MatchingBipartite.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int N, M;
  cin >> N >> M;
  vector<string> S(N);
  int bsize = 0, wsize = 0;
  for (int i = 0; i < N; i++) {
    cin >> S[i];
    for (int j = 0; j < M; j++) {
      bsize += S[i][j] == 'b';
      wsize += S[i][j] == 'w';
    }
  }
  MatchingBipartite g(N * M, N * M);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++)
      if ((i + j) & 1 && S[i][j] == 'b') {
        if (i > 0 && S[i - 1][j] == 'w') g.add_edge(i * M + j, (i - 1) * M + j);
        if (i + 1 < N && S[i + 1][j] == 'w')
          g.add_edge(i * M + j, (i + 1) * M + j);
        if (j > 0 && S[i][j - 1] == 'w') g.add_edge(i * M + j, i * M + j - 1);
        if (j + 1 < M && S[i][j + 1] == 'w')
          g.add_edge(i * M + j, i * M + j + 1);
      }
  }
  int x = g.get_matching().first;
  if (bsize > wsize) swap(bsize, wsize);
  int ans = 100 * x + 10 * (bsize - x) + wsize - bsize;
  cout << ans << endl;
  return 0;
}