// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc208/tasks/abc208_e
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Misc/Automaton.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 string N;
 int K;
 cin >> N >> K;
 int n= N.length();
 std::vector<int> alp= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
 auto tr_le= [&](int s, int q) {
  if (s >= n) return s;
  int c= N[s] - '0';
  if (q > c) return n + 1;
  if (q < c) return n;
  return s + 1;
 };
 auto ac_le= [&](int s) { return true; };
 Automaton dfa_le(alp, 0, tr_le, ac_le, n + 1);
 auto tr_k= [&](int s, int q) -> int {
  auto p= (int64_t)s * q;
  if (p > K) return K + 1;
  return p;
 };
 auto ac_k= [&](int s) { return s <= K; };
 Automaton dfa_k(alp, 1, tr_k, ac_k);
 auto dfa= dfa_le & dfa_k;
 int64_t ans= 0, pw= 1;
 for (int i= 1; i < n; ++i, pw*= 10) ans+= dfa_k.num<int64_t>(i) - pw;
 ans+= dfa.num<int64_t>(n) - pw;
 cout << ans << '\n';
 return 0;
}