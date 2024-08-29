// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc305/tasks/abc305_Ex
// competitive-verifier: TLE 0.5
// https://atcoder.jp/contests/abc305/tasks/abc305_h
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include "src/Optimization/fibonacci_search.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, X, base= 0;
 cin >> N >> X;
 vector<pair<int, int>> AB;
 for (int i= 0; i < N; ++i) {
  int A, B;
  cin >> A >> B;
  if (A == 1) X-= B, base+= B;
  else AB.emplace_back(A, B);
 }
 N= AB.size();
 if (N == 0) return cout << 1 << " " << base << '\n', 0;
 auto comp= [&](const pair<int, int> &l, const pair<int, int> &r) { return (long long)l.second * (r.first - 1) < (long long)r.second * (l.first - 1); };
 vector<int> w[N];
 for (int i= 0; i < N; ++i) {
  vector<pair<int, int>> s;
  for (int j= i; j < N; ++j) {
   s.emplace_back(AB[j]);
   for (int k= s.size(); --k;) {
    if (comp(s[k - 1], s[k])) break;
    swap(s[k - 1], s[k]);
   }
   long long x= 0;
   for (auto &c: s) x= c.first * x + c.second;
   if (x > X) break;
   w[i].push_back(x);
  }
 }
 auto f= [&](long long p) {
  long long dp[N + 1];
  fill_n(dp, N + 1, 1e18);
  dp[0]= 0;
  for (int i= 0; i < N; ++i)
   for (int j= 0, e= w[i].size(); j < e; ++j) dp[i + j + 1]= min(dp[i + j + 1], dp[i] + w[i][j] + p);
  return dp[N];
 };
 auto g= [&](long long p) { return (long double)(f(p) - X) / p; };
 auto [p, DD]= fibonacci_search<MAXIMIZE>(g, 1, X);
 long long D= ceil(DD);
 cout << D << " " << f(p) - p * D + base << '\n';
 return 0;
}