// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1494
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 string S;
 cin >> S;
 int n= S.length();
 Graph g(N, N - 1);
 vector<char> c(N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i] >> c[i], --g[i];
 using Data= vector<int>;
 auto put_edge= [&](int, int e, Data d) {
  for (int i= n; i--;)
   if (S[i] == c[e]) d[i + 1]= max(d[i + 1], d[i] + 1);
  for (int i= 0; i < n; ++i) d[i + 1]= max(d[i + 1], d[i]);
  return d;
 };
 auto op= [&](Data l, const Data &r) {
  for (int i= 0; i <= n; ++i) l[i]= max(l[i], r[i]);
  return l;
 };
 auto put_vertex= [&](int, const Data &d) { return d; };
 int ans= 0;
 for (const auto &v: Rerooting<Data>(g, put_edge, op, Data(n + 1, 0), put_vertex)) ans= max(ans, v[n]);
 cout << ans << '\n';
 return 0;
}