// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc182/tasks/abc182_e
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/DataStructure/RangeSet.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int H, W, N, M;
 cin >> H >> W >> N >> M;
 int A[N], B[N], C[M], D[M];
 for (int i= 0; i < N; ++i) cin >> A[i] >> B[i], --A[i], --B[i];
 for (int i= 0; i < M; ++i) cin >> C[i] >> D[i], --C[i], --D[i];
 bool g[H][W];
 for (int i= H; i--;) fill_n(g[i], W, 0);
 {
  RangeSet<int> rs[H], res[H];
  for (int i= H; i--;) rs[i].insert(0, W - 1);
  for (int i= M; i--;) rs[C[i]].erase(D[i]);
  for (int i= N; i--;) res[A[i]].insert(rs[A[i]].covered_by(B[i]));
  for (int i= H; i--;)
   for (int j= W; j--;) g[i][j]|= res[i].covered_by(j);
 }
 {
  RangeSet<int> rs[W], res[W];
  for (int i= W; i--;) rs[i].insert(0, H - 1);
  for (int i= M; i--;) rs[D[i]].erase(C[i]);
  for (int i= N; i--;) res[B[i]].insert(rs[B[i]].covered_by(A[i]));
  for (int j= W; j--;)
   for (int i= H; i--;) g[i][j]|= res[j].covered_by(i);
 }
 int ans= 0;
 for (int i= H; i--;)
  for (int j= W; j--;) ans+= g[i][j];
 cout << ans << '\n';
 return 0;
}
