// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/685
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <array>
#include "src/Math/ModInt.hpp"
#include "src/Misc/Automaton.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 using symbol_t= array<int, 2>;
 vector<symbol_t> alp= {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
 int64_t N;
 cin >> N;
 auto tr_le= [&](int64_t s, symbol_t c) { return (s - c[1] + 2) / 2 - 1; };
 Automaton dfa_le(alp, N, tr_le, [](int64_t) { return true; }, int64_t(-1));
 auto tr_xy= [&](bool s, symbol_t c) {
  if (c[0] < c[1]) s= 1;
  else if (c[0] > c[1]) s= 0;
  return s;
 };
 Automaton dfa_xy(alp, false, tr_xy, [](bool s) { return s; });
 auto tr_ax= [&](bool s, symbol_t c) {
  bool a= c[0] & c[1], x= c[0] ^ c[1];
  if (a < x) s= 1;
  else if (a > x) s= 0;
  return s;
 };
 Automaton dfa_ax(alp, false, tr_ax, [](bool s) { return s; });
 auto tr_xo= [&](bool s, symbol_t c) {
  bool x= c[0] ^ c[1], o= c[0] | c[1];
  if (x < o) s= 1;
  else if (x > o) s= 0;
  return s;
 };
 Automaton dfa_xo(alp, false, tr_xo, [](bool s) { return s; });
 auto dfa= dfa_le & dfa_xy & dfa_ax & dfa_xo;
 cout << dfa.num<Mint>(60) << '\n';
 return 0;
}