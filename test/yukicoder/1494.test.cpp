#define PROBLEM "https://yukicoder.me/problems/no/1494"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "src/Graph/rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 string S;
 cin >> S;
 int n= S.length();
 Tree<char> tree(N);
 for (int i= 0; i < N - 1; ++i) {
  int u, v;
  char c;
  cin >> u >> v >> c;
  tree.add_edge(--u, --v, c);
 }
 tree.build();
 using Data= vector<int>;
 auto f_ee= [&](const Data &l, const Data &r) {
  Data ret(n + 1);
  for (int i= 0; i <= n; ++i) ret[i]= max(l[i], r[i]);
  return ret;
 };
 auto f_ve= [&](Data d, int, const auto &e) {
  for (int i= n; i--;)
   if (S[i] == e.cost) d[i + 1]= max(d[i + 1], d[i] + 1);
  for (int i= 0; i < n; ++i) d[i + 1]= max(d[i + 1], d[i]);
  return d;
 };
 auto f_ev= [&](const Data &d, int) { return d; };
 auto dp= rerooting<Data>(tree, f_ee, f_ve, f_ev, Data(n + 1, 0));
 int ans= 0;
 for (const auto &v: dp) ans= max(ans, v[n]);
 cout << ans << '\n';
 return 0;
}