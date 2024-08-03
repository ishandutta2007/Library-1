// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/1039
#include <iostream>
#include "src/Geometry/Point.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using R= long double;
 for (int N, M; cin >> N >> M && N;) {
  Point<R> D[N];
  R V[N];
  for (int i= 0; i < N; ++i) cin >> D[i] >> V[i];
  int ans[N];
  fill_n(ans, N, 0);
  while (M--) {
   Point<R> FP, FV;
   cin >> FP >> FV;
   Point<R> v[N];
   R t[N], mn= 1e18, FV2= norm2(FV);
   fill_n(t, N, -1);
   int id= -1;
   for (int i= N; i--;) {
    if (FP == D[i]) {
     mn= t[i]= 0, id= i;
     break;
    }
    R a= FV2 - V[i] * V[i], b= dot(FP - D[i], FV), c= norm2(FP - D[i]);
    if (sgn(a) == 0) {
     if (sgn(b) != 0) t[i]= -c / (2 * b);
    } else {
     b/= a, c/= a;
     R D= b * b - c;
     if (D >= 0) {
      D= sqrt(D);
      if (-b + D >= 0) t[i]= -b + D;
      if (-b - D >= 0) t[i]= -b - D;
     }
    }
    if (t[i] >= 0) {
     if (mn > t[i]) mn= t[i], id= i;
     v[i]= FP + t[i] * FV - D[i], v[i]/= norm(v[i]);
    }
   }
   assert(id != -1);
   ++ans[id];
   for (int i= N; i--;) {
    if (t[i] < 0) continue;
    D[i]+= mn * V[i] * v[i];
   }
  }
  for (int i= 0; i < N; ++i) cout << ans[i] << " \n"[i == N - 1];
 }
 return 0;
}