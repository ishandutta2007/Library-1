// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1106
#include <iostream>
#include <vector>
#include "src/Misc/Automaton.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 vector alp= {-2, -1, 0, 1, 2};
 int64_t N;
 cin >> N;
 auto tr_le= [&](int64_t s, int c) { return (s - c + 5) / 5 - 1; };
 Automaton dfa_le(
     alp, N, tr_le, [](int64_t) { return true; }, int64_t(-1));
 auto tr_pos= [&](bool s, int c) {
  if (c > 0) s= 1;
  else if (c < 0) s= 0;
  return s;
 };
 Automaton dfa_pos(alp, false, tr_pos, [](bool s) { return s; });
 auto tr_eq= [&](int s, int c) {
  if (abs(s) > 100) return s;
  return s + c;
 };
 Automaton dfa_eq(alp, 0, tr_eq, [](int s) { return s == 0; });
 auto dfa= dfa_le & dfa_pos & dfa_eq;
 cout << dfa.num<int64_t>(60) << '\n';
 return 0;
}