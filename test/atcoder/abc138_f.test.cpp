// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc138/tasks/abc138_f
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include <array>
#include "src/Misc/Automaton.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 int64_t L, R;
 cin >> L >> R;
 using symbol_t= array<bool, 2>;
 vector<symbol_t> alp= {{0, 0}, {0, 1}, {1, 1}};
 auto tr_le= [](int64_t s, symbol_t c) { return (s - c[1] + 2) / 2 - 1; };
 auto tr_ge= [](int64_t s, symbol_t c) { return (s - c[0] + 1) / 2; };
 auto ac= [](int64_t s) { return s == 0; };
 Automaton dfa_le(alp, R, tr_le, ac, int64_t(-1)), dfa_ge(alp, L, tr_ge, ac, int64_t(-1));
 auto tr_len= [](bool s, symbol_t c) {
  if (c[0] == 1 && c[1] == 1) s= 1;
  if (c[0] != c[1]) s= 0;
  return s;
 };
 auto ac_len= [](bool s) { return s; };
 Automaton dfa_len(alp, true, tr_len, ac_len);
 auto dfa= dfa_le & dfa_ge & dfa_len;
 cout << dfa.num<Mint>(60) << '\n';
 return 0;
}