// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc202/tasks/abc202_f
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Geometry/Polygon.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using Mint= ModInt<int(1e9) + 7>;
 int N;
 cin >> N;
 vector<Point<int>> ps(N);
 for (int i= 0; i < N; ++i) cin >> ps[i];
 sort(ps.begin(), ps.end());
 int parity[N][N][N], inside[N][N][N];
 for (int i= N; i--;)
  for (int j= N; j--;) {
   if (i == j) continue;
   for (int k= N; k--;) {
    if (i == k || j == k) continue;
    Polygon<int> g({ps[i], ps[j], ps[k]});
    parity[i][j][k]= g.area2() & 1, inside[i][j][k]= 0;
    for (int l= N; l--;) {
     if (l == i || l == j || l == k) continue;
     inside[i][j][k]+= g.where(ps[l]) != -1;
    }
   }
  }
 Mint ans= 0, pw[N + 1];
 pw[0]= 1;
 for (int i= 0; i < N; ++i) pw[i + 1]= pw[i] + pw[i];
 for (int must= N; must--;) {
  Mint dp[N][N][2][2];
  for (int i= must; i < N; ++i)
   for (int j= must; j < N; ++j)
    for (int k= 2; k--;) dp[i][j][k][0]= dp[i][j][k][1]= 0;
  for (int i= must + 1; i < N; ++i) dp[must][i][0][0]= dp[must][i][0][1]= 1;
  for (int i= must; i < N; ++i)
   for (int j= i + 1; j < N; ++j)
    for (int k= 2; k--;)
     for (int l= j + 1; l < N; ++l) {
      bool f= ccw(ps[i], ps[j], ps[l]) == CLOCKWISE;
      dp[j][l][k ^ parity[must][j][l]][f]+= dp[i][j][k][f] * pw[inside[must][j][l]];
     }
  for (int j= must + 1; j < N; ++j)
   for (int k= 2; k--;) {
    Mint up= 0, lo= 0;
    for (int i= must; i < j; ++i) up+= dp[i][j][k][0], lo+= dp[i][j][k][1];
    ans+= up * lo;
   }
 }
 cout << ans - N * (N - 1) / 2 << '\n';
 return 0;
}