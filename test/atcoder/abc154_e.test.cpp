// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc154/tasks/abc154_e
#include <iostream>
#include <vector>
#include <string>
#include "src/Misc/Automaton.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 string N;
 int K;
 cin >> N >> K;
 int n= N.length();
 vector alp= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
 auto tr_le= [&](int s, int c) {
  if (s >= n) return s;
  int d= N[s] - '0';
  if (c < d) return n;
  if (c > d) return n + 1;
  return s + 1;
 };
 auto ac_le= [](int) { return true; };
 Automaton dfa_le(alp, 0, tr_le, ac_le, n + 1);
 auto tr_0= [&](int s, int c) { return s + bool(c); };
 auto ac_0= [&](int s) { return s == K; };
 Automaton dfa_0(alp, 0, tr_0, ac_0, K + 1);
 auto dfa= dfa_le & dfa_0;
 cout << dfa.num<int>(n) << '\n';
 return 0;
}