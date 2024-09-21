// competitive-verifier: PROBLEM https://atcoder.jp/contests/arc070/tasks/arc066_D
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/arc066/tasks/arc066_b
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
 long long N;
 cin >> N;
 using symbol_t= array<bool, 2>;
 vector<symbol_t> alp= {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
 auto tr= [&](int s, symbol_t c) {
  auto [u, v]= c;
  set<int> ret;
  for (int a= 2; a--;)
   for (int b= 2; b--;)
    if ((a ^ b) == u && (a + b + s) % 2 == v) ret.insert((a + b + s) / 2);
  return ret;
 };
 auto ac= [&](int s) { return s == 0; };
 Automaton nfa(alp, 0, tr, ac);
 auto tr_le0= [&](long long s, symbol_t c) { return (s - c[0] + 2) / 2 - 1; };
 auto tr_le1= [&](long long s, symbol_t c) { return (s - c[1] + 2) / 2 - 1; };
 auto ac_le= [&](long long s) { return s == 0; };
 Automaton dfa_le0(alp, N, tr_le0, ac_le), dfa_le1(alp, N, tr_le1, ac_le);
 cout << (nfa & dfa_le0 & dfa_le1).num<Mint>(60) << '\n';
 return 0;
}