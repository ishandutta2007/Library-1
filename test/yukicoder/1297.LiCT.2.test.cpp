// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1297
#include <iostream>
#include <array>
#include <algorithm>
#include "src/Optimization/LiChaoTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long N, C;
 cin >> N >> C;
 long long a[N], b[N];
 for (int i= 0; i < N; ++i) cin >> a[i] >> b[i];
 auto f1= [&](int i, int j, long long d) {
  long long x= i - j;
  return d + a[i - 1] * x + C * (x - 1) * x / 2 + b[i - 1];
 };
 auto f2= [&](int i, int j, long long d) {
  long long x= i - j;
  return d + a[j - 1] * x + C * x * (x + 1) / 2;
 };
 LiChaoTree lct1(f1, 1, N + 1);
 LiChaoTree lct2(f2, 1, N + 1);
 auto tree1= lct1.make_tree<MINIMIZE>();
 auto tree2= lct2.make_tree<MINIMIZE>();
 tree1.insert(0, 0);
 for (int i= 1; i <= N; ++i) {
  auto dp1= tree1.query(i).first;
  tree2.insert(i, dp1, i);
  auto dp2= tree2.query(i).first;
  if (i == N) {
   cout << dp2 << '\n';
   break;
  }
  tree1.insert(i, dp2, i + 1);
 }
 return 0;
}