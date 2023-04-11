#define PROBLEM "https://yukicoder.me/problems/no/362"
#include <iostream>
#include <vector>
#include <array>
#include "src/Misc/Automaton.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 vector alp= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
 auto tr_le= [&](int64_t s, int c) { return (s - c + 10) / 10 - 1; };
 auto ac_le= [&](int64_t) { return true; };
 using state_t= array<int, 2>;
 auto tr_kado= [&](state_t s, int c) -> state_t {
  if (s[0] == -1) {
   if (s[1] == c) return {-2, -2};
   else return {s[1], c};
  }
  if (s[0] == c || s[1] == c) return {-2, -2};
  auto [mi, mx]= minmax({s[0], s[1], c});
  if (s[1] == mi || s[1] == mx) return {s[1], c};
  return {-2, -2};
 };
 auto ac_kado= [&](state_t s) { return true; };
 Automaton dfa_kado(alp, state_t{-1, -1}, tr_kado, ac_kado, state_t{-2, -2});
 auto tr_0= [&](bool, int c) -> bool { return c; };
 Automaton dfa_0(alp, false, tr_0, [](bool s) { return s; });
 auto dfa1= dfa_kado & dfa_0;
 int T;
 cin >> T;
 while (T--) {
  int64_t K;
  cin >> K;
  int64_t l= 101, h= 37294859064823;
  while (h - l > 1) {
   int64_t X= (h + l) / 2;
   int n= 0;
   for (auto m= X; m; m/= 10) ++n;
   Automaton dfa_le(alp, X, tr_le, ac_le, int64_t(-1));
   auto dfa= dfa1 & dfa_le;
   int64_t sum= dfa.num<int64_t>(n);
   for (int i= 3; i < n; ++i) sum+= dfa1.num<int64_t>(i);
   if (sum < K) l= X;
   else h= X;
  }
  cout << h << '\n';
 }
 return 0;
}