// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc276/tasks/abc276_Ex
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/abc276/tasks/abc276_h
#include <iostream>
#include <vector>
#include <unordered_map>
#include "src/LinearAlgebra/LU_Decomposition.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 int a[Q], b[Q], c[Q], d[Q], e[Q];
 for (int i= 0; i < Q; ++i) cin >> a[i] >> b[i] >> c[i] >> d[i] >> e[i], --a[i], --b[i], --c[i], --d[i];
 unordered_map<int, int> mp;
 vector<int> pm;
 auto id= [&](int x) -> int {
  if (auto it= mp.find(x); it == mp.end()) return mp[x]= pm.size(), pm.push_back(x), pm.size() - 1;
  else return it->second;
 };
 Matrix<int> cum(N, N);
 int m= 0;
 for (int i= 0; i < Q; ++i) {
  if (!e[i]) continue;
  ++cum[a[i]][c[i]];
  if (b[i] + 1 < N) --cum[b[i] + 1][c[i]];
  if (d[i] + 1 < N) --cum[a[i]][d[i] + 1];
  if (b[i] + 1 < N && d[i] + 1 < N) ++cum[b[i] + 1][d[i] + 1];
  id(b[i] * N + d[i]);
  if (a[i]) id((a[i] - 1) * N + d[i]);
  if (c[i]) id(b[i] * N + (c[i] - 1));
  if (a[i] && c[i]) id((a[i] - 1) * N + (c[i] - 1));
  ++m;
 }
 if (m == 0) {
  cout << "Yes\n";
  for (int i= 0; i < N; ++i)
   for (int j= 0; j < N; ++j) cout << 0 << " \n"[j + 1 == N];
  return 0;
 }
 for (int i= 0; i < N; ++i)
  for (int j= 1; j < N; ++j) cum[i][j]+= cum[i][j - 1];
 for (int i= 1; i < N; ++i)
  for (int j= 0; j < N; ++j) cum[i][j]+= cum[i - 1][j];
 for (int i= N; i--;)
  for (int j= N; j--;) cum[i][j]= !!cum[i][j];
 auto f= cum;
 for (int i= 0; i < N; ++i)
  for (int j= 1; j < N; ++j) cum[i][j]+= cum[i][j - 1];
 for (int i= 1; i < N; ++i)
  for (int j= 0; j < N; ++j) cum[i][j]+= cum[i - 1][j];
 int n= pm.size();
 Vector<bool> r(m);
 Matrix<bool> A(m, n);
 for (int i= 0, j= 0; i < Q; ++i) {
  if (!e[i]) {
   int cnt= cum[b[i]][d[i]];
   if (a[i]) cnt-= cum[a[i] - 1][d[i]];
   if (c[i]) cnt-= cum[b[i]][c[i] - 1];
   if (a[i] && c[i]) cnt+= cum[a[i] - 1][c[i] - 1];
   if (cnt == (b[i] - a[i] + 1) * (d[i] - c[i] + 1)) return cout << "No" << '\n', 0;
   continue;
  }
  A[j][id(b[i] * N + d[i])]= 1;
  if (a[i]) A[j][id((a[i] - 1) * N + d[i])]= 1;
  if (c[i]) A[j][id(b[i] * N + (c[i] - 1))]= 1;
  if (a[i] && c[i]) A[j][id((a[i] - 1) * N + (c[i] - 1))]= 1;
  r[j]= e[i] - 1;
  ++j;
 }
 auto x= LU_Decomposition(A).linear_equations(r);
 if (!x) return cout << "No" << '\n', 0;
 cout << "Yes" << '\n';
 Matrix<bool> X(N, N);
 for (int k= n; k--;) X[pm[k] / N][pm[k] % N]= x[k];
 for (int i= N; i-- > 1;)
  for (int j= N; j--;) X[i][j]^= X[i - 1][j];
 for (int i= N; i--;)
  for (int j= N; j-- > 1;) X[i][j]^= X[i][j - 1];
 for (int i= 0; i < N; ++i)
  for (int j= 0; j < N; ++j) cout << (f[i][j] ? X[i][j] + 1 : 0) << " \n"[j + 1 == N];
 return 0;
}