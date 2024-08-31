// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc213/tasks/abc213_f
// competitive-verifier: TLE 1
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include "src/String/SuffixTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 string S;
 cin >> S;
 SuffixArray sa(S);
 LCPArray lcp(sa);
 SuffixTree st(sa, lcp);
 int n= st.size();
 vector<long long> sum(n + 1);
 for (int s= 0; s < n; ++s) {
  int v= st.tree.to_vertex(s);
  auto [l, r, h, hh]= st[v];
  sum[s + 1]= sum[s] + (long long)(hh - h) * (r - l);
 }
 for (int i= 0; i < N; ++i) {
  long long ans= 0;
  int v= st.substr(i);
  for (auto [a, b]: st.tree.path(0, v)) {
   ans+= sum[b + 1] - sum[a];
  }
  cout << ans << '\n';
 }
 return 0;
}