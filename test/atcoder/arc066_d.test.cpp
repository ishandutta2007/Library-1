#define PROBLEM "https://atcoder.jp/contests/arc066/tasks/arc066_b"
#include <bits/stdc++.h>
#include "src/Automaton/dfa_dp.hpp"
#include "src/Automaton/DFA_Le.hpp"
#include "src/Automaton/dfa_operations.hpp"
#include "src/Automaton/NFA_to_DFA.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

struct XorSum {
  using state_t = int;
  using symbol_t = std::pair<int, int>;
  std::set<symbol_t> alphabet() const {
    return {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
  }
  state_t initial_state() const { return 0; }
  std::set<state_t> transition(const state_t s, const symbol_t &c) const {
    const auto &[u, v] = c;
    std::set<state_t> ret;
    for (int a = 0; a < 2; a++)
      for (int b = 0; b < 2; b++)
        if (((a ^ b) == u) && ((a + b + s) % 2 == v))
          ret.insert((a + b + s) / 2);
    return ret;
  }
  std::set<state_t> eps_transition(const state_t) const { return {}; }
  bool is_accept(const state_t s) const { return s == 0; }
} xor_sum;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = ModInt<int(1e9 + 7)>;
  long long N;
  cin >> N;
  auto alp = xor_sum.alphabet();
  auto dfa_le = DFA_Le(N, 2, 60);
  using S = typename XorSum::symbol_t;
  auto dfa_le_l =
      DFA_SymbolMap(dfa_le, alp, [](const S &a) { return a.first; });
  auto dfa_le_r =
      DFA_SymbolMap(dfa_le, alp, [](const S &a) { return a.second; });
  cout << dfa_dp<Mint>(NFA_to_DFA(xor_sum) & dfa_le_l & dfa_le_r, 60) << '\n';
  return 0;
}