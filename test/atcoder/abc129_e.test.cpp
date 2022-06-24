#define PROBLEM "https://atcoder.jp/contests/abc129/tasks/abc129_e"
#include <bits/stdc++.h>
#include "src/Automaton/dfa_dp.hpp"
#include "src/Automaton/DFA_Le.hpp"
#include "src/Automaton/dfa_operations.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

struct DFA_Bin {
  using state_t = int;
  using symbol_t = std::tuple<int, int, int>;
  std::set<symbol_t> alphabet() const {
    return {{0, 0, 0}, {0, 1, 1}, {1, 0, 1}, {1, 1, 0}};  // because u^v=w
  }
  state_t initial_state() const { return 0; }
  state_t transition(const state_t s, const symbol_t &a) const {
    if (s == 2) return 2;
    const auto &[u, v, w] = a;
    return (s ^ u ^ v == w) ? ((s + u + v) / 2) : 2;
  }
  bool is_accept(const state_t s) const { return s == 0; }
} dfa_bin;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = ModInt<int(1e9 + 7)>;
  string L;
  cin >> L;
  auto dfa_le = DFA_SymbolMap(
      DFA_Le(L, 2), dfa_bin.alphabet(),
      [](const std::tuple<int, int, int> &a) { return std::get<2>(a); });
  cout << dfa_dp<Mint>(dfa_bin & dfa_le, L.length()) << '\n';
  return 0;
}