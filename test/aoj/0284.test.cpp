#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Prelim/0284"
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include "src/Geometry/Point.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 int N;
 cin >> N;
 vector<Point<int>> ps(N);
 for (int i= 0; i < N; ++i) cin >> ps[i];
 int ord[N];
 iota(ord, ord + N, 0), sort(ord, ord + N, [&](int i, int j) { return ps[i] < ps[j]; });
 static constexpr int INF= 5e8;
 int dp[N + 1][N][N][N], pre[N + 1][N][N][N];
 for (int k= 2; k <= N; ++k)
  for (int i= N; i--;)
   for (int j= N; j--;)
    for (int l= N; l--;) dp[k][i][j][l]= INF;
 for (int i= N; i--;)
  for (int j= i; j--;) dp[2][i][i][j]= 0;
 for (int k= 3; k <= N; ++k)
  for (int i= N; i--;) {
   auto o= ps[ord[i]];
   for (int j= 0; j <= i; ++j) {
    auto p= ps[ord[j]];
    for (int l= i; l--;) {
     if (j == l) continue;
     if (dp[k - 1][i][j][l] == INF) continue;
     auto q= ps[ord[l]];
     for (int m= i; m--;) {
      if (j == m || l == m) continue;
      auto r= ps[ord[m]];
      if (ccw(p, q, r) != COUNTER_CLOCKWISE) continue;
      if (ccw(q, r, o) != COUNTER_CLOCKWISE) continue;
      int a= cross(r - q, o - q) + dp[k - 1][i][j][l];
      if (dp[k][i][l][m] > a) dp[k][i][l][m]= a, pre[k][i][l][m]= j;
     }
    }
   }
  }
 int Q;
 cin >> Q;
 while (Q--) {
  int k;
  cin >> k;
  int mn= INF, ii= -1, jj= -1, ll= -1;
  for (int i= N; i--;)
   for (int j= i; j--;)
    for (int l= i; l--;)
     if (mn > dp[k][i][j][l]) mn= dp[k][i][j][l], ii= i, jj= j, ll= l;
  if (ii == -1) {
   cout << "NA" << '\n';
   continue;
  }
  vector<int> ch;
  for (int kk= k; kk >= 3; --kk) {
   ch.push_back(ord[ll]);
   int p= pre[kk][ii][jj][ll];
   ll= jj, jj= p;
  }
  ch.push_back(ord[ll]), ch.push_back(ord[ii]);
  reverse(ch.begin(), ch.end());
  int st= 0;
  for (int i= 0; i < k; ++i)
   if (auto p= ps[ch[st]], q= ps[ch[i]]; p.y > q.y || (p.y == q.y && p.x > q.x)) st= i;
  for (int i= 0; i < k; ++i) cout << ch[(i + st) % k] + 1 << " \n"[i + 1 == k];
 }
 return 0;
}