// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc228/tasks/abc228_f
// competitive-verifier: TLE 0.5

// kdt は TLE

#include <iostream>
#include <vector>
#include <array>
#include "src/DataStructure/SegmentTree_2D.hpp"
using namespace std;
struct RMQ {
 using T= long long;
 static T ti() { return 0; }
 static T op(T l, T r) { return max(l, r); }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int H, W, h1, w1, h2, w2;
 cin >> H >> W >> h1 >> w1 >> h2 >> w2;
 h2= min(h2, h1), w2= min(w2, w1);
 vector A(H + 1, vector<long long>(W + 1));
 for (int i= 1; i <= H; ++i)
  for (int j= 1; j <= W; ++j) cin >> A[i][j];
 for (int i= 0; i <= H; ++i)
  for (int j= 0; j < W; ++j) A[i][j + 1]+= A[i][j];
 for (int j= 0; j <= W; ++j)
  for (int i= 0; i < H; ++i) A[i + 1][j]+= A[i][j];
 auto sum= [&](int l, int r, int u, int d) { return A[r][d] - A[l][d] - A[r][u] + A[l][u]; };
 vector<tuple<int, int, long long>> v;
 for (int i= 0; i + h2 <= H; ++i)
  for (int j= 0; j + w2 <= W; ++j) v.emplace_back(i, j, sum(i, i + h2, j, j + w2));
 SegmentTree_2D<int, RMQ> seg(v);
 long long ans= 0;
 for (int i= 0; i + h1 <= H; ++i)
  for (int j= 0; j + w1 <= W; ++j) ans= max(ans, sum(i, i + h1, j, j + w1) - seg.prod(i, i + h1 - h2 + 1, j, j + w1 - w2 + 1));
 cout << ans << '\n';
 return 0;
}
