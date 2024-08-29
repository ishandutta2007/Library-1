// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc121/tasks/abc121_d
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Misc/Automaton.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 long long A, B;
 cin >> A >> B;
 vector alp= {0, 1};
 auto tr_le= [&](int s, int c) {
  if (s < 0) return s;
  int d= (B >> s) & 1;
  if (c < d) return -1;
  if (c > d) return -2;
  return s - 1;
 };
 auto tr_ge= [&](int s, int c) {
  if (s < 0) return s;
  int d= (A >> s) & 1;
  if (c > d) return -1;
  if (c < d) return -2;
  return s - 1;
 };
 auto ac= [](int) { return true; };
 Automaton dfa_a(alp, 39, tr_ge, ac, -2), dfa_b(alp, 39, tr_le, ac, -2);
 auto dfa= dfa_a & dfa_b;
 using T= pair<bool, long long>;
 auto op= [](const T &l, const T &r) { return T{l.first ^ r.first, l.second ^ r.second}; };
 auto f= [](T x, int c, int) {
  x.second<<= 1;
  if (c) x.second|= x.first;
  return x;
 };
 cout << dfa.dp_run(40, op, T{0, 0}, f, T{1, 0}).second << '\n';
 return 0;
}