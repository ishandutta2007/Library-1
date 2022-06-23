#define PROBLEM "https://atcoder.jp/contests/abc138/tasks/abc138_f"
#include <bits/stdc++.h>
#include "src/Automaton/dfa_dp.hpp"
#include "src/Automaton/DFA_Le.hpp"
#include "src/Automaton/dfa_operations.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

struct DFA_SameLen {
  using state_t = int;
  using symbol_t = pair<int, int>;
  std::set<symbol_t> alphabet() const { return {{0, 0}, {0, 1}, {1, 1}}; }
  state_t initial_state() const { return 0; }
  state_t transition(const state_t s, const symbol_t &a) const {
    const auto &[u, v] = a;
    if (u == 1 && v == 1) return 0;
    if (u != v) return 1;
    return s;
  }
  bool is_accept(const state_t s) const { return s == 0; }
} dfa_samelen;
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = ModInt<int(1e9 + 7)>;
  long long L, R;
  cin >> L >> R;
  auto dfa_le_r = DFA_Le(R, 2, 61);
  auto dfa_ge_l = -DFA_Le(L - 1, 2, 61);
  auto alp = dfa_samelen.alphabet();
  using S = typename DFA_SameLen::symbol_t;
  auto dfa_x_le =
      DFA_SymbolMap(dfa_le_r, alp, [](const S &a) { return a.first; });
  auto dfa_y_le =
      DFA_SymbolMap(dfa_le_r, alp, [](const S &a) { return a.second; });
  auto dfa_x_ge =
      DFA_SymbolMap(dfa_ge_l, alp, [](const S &a) { return a.first; });
  auto dfa_y_ge =
      DFA_SymbolMap(dfa_ge_l, alp, [](const S &a) { return a.second; });
  auto dfa = dfa_x_le & dfa_x_ge & dfa_y_le & dfa_y_ge & dfa_samelen;
  cout << dfa_dp<Mint>(dfa, 61) << '\n';
  return 0;
}