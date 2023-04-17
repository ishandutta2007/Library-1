#define PROBLEM "https://atcoder.jp/contests/agc037/tasks/agc037_d"
#include <iostream>
#include <vector>
#include "src/Graph/BipartiteEdgeColoring.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 BipartiteEdgeColoring graph(N, N);
 int A[N][M];
 for (int i= 0; i < N; ++i)
  for (int j= 0; j < M; ++j) {
   cin >> A[i][j], A[i][j];
   graph.add_edge(i, (A[i][j] - 1) / M);
  }
 auto col= graph.edge_coloring();
 int B[M][N];
 for (int i= 0; i < N; ++i)
  for (int j= 0; j < M; ++j) {
   B[col[i * M + j]][i]= A[i][j];
  }
 for (int i= 0; i < N; ++i)
  for (int j= 0; j < M; ++j) cout << B[j][i] << " \n"[j == M - 1];
 for (int j= 0; j < M; ++j) sort(B[j], B[j] + N);
 for (int i= 0; i < N; ++i)
  for (int j= 0; j < M; ++j) cout << B[j][i] << " \n"[j == M - 1];
 return 0;
}