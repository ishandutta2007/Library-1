#define PROBLEM "https://atcoder.jp/contests/abc138/tasks/abc138_f"
#include <bits/stdc++.h>
#include "src/Automaton/dfa_dp.hpp"
#include "src/Automaton/DFA_Inequality.hpp"
#include "src/Automaton/dfa_operations.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

using Pii= pair<int, int>;
struct DFA_SameLen {
 using symbol_t= Pii;
 std::vector<symbol_t> alphabet() const { return {{0, 0}, {0, 1}, {1, 1}}; }
 inline int initial_state() const { return 0; }
 inline int transition(int s, const symbol_t &a, int) const {
  const auto &[u, v]= a;
  if (s == 1) return 1;
  if (u != v) return -1;
  return u == 1;
 }
 inline bool is_accept(int s) const { return s >= 0; }
 inline int state_size() const { return 2; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 long long L, R;
 cin >> L >> R;
 auto dfa_samelen= DFA_SameLen();
 auto alp= dfa_samelen.alphabet();
 auto dfa_y_le= DFA_SymbolMap(DFA_Inequality(R, 2, 61), alp, [](const Pii &a) { return a.second; });
 auto dfa_x_ge= DFA_SymbolMap(DFA_Inequality<true>(L, 2, 61), alp, [](const Pii &a) { return a.first; });
 cout << dfa_dp<Mint>(dfa_x_ge & dfa_y_le & dfa_samelen, 61) << '\n';
 return 0;
}