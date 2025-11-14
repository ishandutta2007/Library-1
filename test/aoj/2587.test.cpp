// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/JAG/Prelim/2587
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <set>
#include <array>
#include "src/Misc/Automaton.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N;
 vector<int> low, high;
 vector alp= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
 while (cin >> N && N != 0) {
  low.resize(N), high.resize(N);
  for (int i= 0; i < N; ++i) cin >> low[i] >> high[i];
  using state_t= array<int, 2>;
  auto tr= [&](state_t s, int c) -> set<state_t> {
   auto [i, j]= s;
   if (i >= N) return {};
   auto [l1, l0]= div(low[i], 10);
   auto [h1, h0]= div(high[i], 10);
   if (j == 1) {
    if (l0 <= c) return {{i + 1, 0}};
   } else if (j == 2) return {{i + 1, 0}};
   else if (j == 3) {
    if (c <= h0) return {{i + 1, 0}};
   } else if (j == 4) {
    if (l0 <= c && c <= h0) return {{i + 1, 0}};
   } else {
    if (c == 0) return {};
    if (l1 == h1) {
     if (c == l1) return {{i, 4}};
    } else {
     if (c == l1) return {{i, 1}};
     if (l1 < c && c < h1) return {{i, 2}};
     if (c == h1) return {{i, 3}};
    }
   }
   return {};
  };
  auto eps= [&](state_t s) -> set<state_t> {
   auto [i, j]= s;
   if (i >= N) return {};
   if (j == 0 && (low[i] / 10) == 0) {
    if ((high[i] / 10) == 0) return {{i, 4}};
    else return {{i, 1}};
   }
   return {};
  };
  auto ac= [&](state_t s) { return s[0] == N && s[1] == 0; };
  Automaton nfa(alp, state_t{0, 0}, tr, ac, eps);
  int64_t ans= 0;
  for (int i= N; i <= 2 * N; ++i) ans+= nfa.num<int64_t>(i);
  cout << ans << '\n';
 }
 return 0;
}