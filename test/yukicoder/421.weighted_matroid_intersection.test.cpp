// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/421
// 重みなし問題をあえて重み付きアルゴリズムで
// 2部マッチング(分割+分割) (2500+2500 頂点)
// |台集合|<=5000
// |共通独立集合|<=2500
#include <iostream>
#include <vector>
#include <string>
#include "src/Optimization/matroid_intersection.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, M;
 cin >> N >> M;
 vector<string> S(N);
 int bsize= 0, wsize= 0;
 for (int i= 0; i < N; i++) {
  cin >> S[i];
  for (int j= 0; j < M; j++) bsize+= S[i][j] == 'b', wsize+= S[i][j] == 'w';
 }
 vector<vector<int>> partl(N * M), partr(N * M);
 int e= 0;
 auto add_edge= [&](int l, int r) { partl[l].push_back(e), partr[r].push_back(e), ++e; };
 for (int i= 0; i < N; i++) {
  for (int j= 0; j < M; j++)
   if ((i + j) & 1 && S[i][j] == 'b') {
    if (i > 0 && S[i - 1][j] == 'w') add_edge(i * M + j, (i - 1) * M + j);
    if (i + 1 < N && S[i + 1][j] == 'w') add_edge(i * M + j, (i + 1) * M + j);
    if (j > 0 && S[i][j - 1] == 'w') add_edge(i * M + j, i * M + j - 1);
    if (j + 1 < M && S[i][j + 1] == 'w') add_edge(i * M + j, i * M + j + 1);
   }
 }
 if (bsize > wsize) swap(bsize, wsize);
 PartitionMatroid M1(e, partl, vector(N * M, 1)), M2(e, partr, vector(N * M, 1));
 int x= weighted_matroid_intersection<MAXIMIZE>(e, M1, M2, vector(e, 1)).size() - 1;
 int ans= 100 * x + 10 * (bsize - x) + wsize - bsize;
 cout << ans << '\n';
 return 0;
}
