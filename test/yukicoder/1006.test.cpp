// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1006
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/NumberTheory/tables.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int X;
 cin >> X;
 auto f= [&](int N) {
  int d= 1;
  for (auto [p, e]: factorize(N)) d*= e + 1;
  return N - d;
 };
 int best= 1 << 30;
 vector<int> ans;
 for (int A= 1; A < X; ++A) {
  int val= abs(f(A) - f(X - A));
  if (best > val) best= val, ans.clear();
  if (best == val) ans.push_back(A);
 }
 for (auto A: ans) cout << A << ' ' << X - A << '\n';
 return 0;
}