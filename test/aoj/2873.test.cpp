// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2873
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/String/AhoCorasick.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string S;
 cin >> S;
 int N;
 cin >> N;
 vector<string> P(N);
 for (int i= 0; i < N; ++i) cin >> P[i];
 AhoCorasick aho(P);
 int n= S.length();
 int ans= 0;
 for (int i= 0, s= aho.initial_state(); i < n; ++i) {
  int ns= aho.transition(s, S[i]);
  if (aho.is_accept(ns)) ++ans, ns= aho.initial_state();
  s= ns;
 }
 cout << ans << '\n';
 return 0;
}