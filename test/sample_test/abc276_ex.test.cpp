// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/abc276/tasks/abc276_h
// sp judge
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "src/LinearAlgebra/LU_Decomposition.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 if (expected == "No\n") return scout.str() == expected;
 string yes;
 scout >> yes;
 if (yes != "Yes") return false;
 stringstream scin2(in);
 int N, Q;
 scin2 >> N >> Q;
 vector X(N, vector<int>(N));
 for (int i= N; i--;)
  for (int j= N; j--;) scout >> X[i][j];
 while (Q--) {
  int a, b, c, d, e;
  scin2 >> a >> b >> c >> d >> e;
  int x= 1;
  for (int i= a; i <= b; ++i)
   for (int j= c; j <= d; ++j) (x*= X[i - 1][j - 1])%= 3;
  if (x != e) return false;
 }
 return true;
}
namespace TEST {
signed main(stringstream& scin, stringstream& scout) {
 int N, Q;
 scin >> N >> Q;
 int a[Q], b[Q], c[Q], d[Q], e[Q];
 for (int i= 0; i < Q; ++i) scin >> a[i] >> b[i] >> c[i] >> d[i] >> e[i], --a[i], --b[i], --c[i], --d[i];
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
  scout << "Yes\n";
  for (int i= 0; i < N; ++i)
   for (int j= 0; j < N; ++j) scout << 0 << " \n"[j + 1 == N];
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
   if (cnt == (b[i] - a[i] + 1) * (d[i] - c[i] + 1)) return scout << "No" << '\n', 0;
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
 if (!x) return scout << "No" << '\n', 0;
 scout << "Yes" << '\n';
 Matrix<bool> X(N, N);
 for (int k= n; k--;) X[pm[k] / N][pm[k] % N]= x[k];
 for (int i= N; i-- > 1;)
  for (int j= N; j--;) X[i][j]^= X[i - 1][j];
 for (int i= N; i--;)
  for (int j= N; j-- > 1;) X[i][j]^= X[i][j - 1];
 for (int i= 0; i < N; ++i)
  for (int j= 0; j < N; ++j) scout << (f[i][j] ? X[i][j] + 1 : 0) << " \n"[j + 1 == N];
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "2 3\n1 1 1 2 0\n1 2 2 2 1\n2 2 1 2 2\n", "Yes\n0 2\n1 2\n"));
 assert(test(TEST::main, "4 4\n1 4 1 4 0\n1 4 1 4 1\n1 4 1 4 2\n1 4 1 4 0\n", "No\n"));
 return 0;
}