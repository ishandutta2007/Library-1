#define PROBLEM "https://atcoder.jp/contests/abc208/tasks/abc208_e"
#include <bits/stdc++.h>
#include "src/Automaton/dfa_dp.hpp"
#include "src/Automaton/DFA_Inequality.hpp"
#include "src/Automaton/dfa_operations.hpp"
#include "src/Automaton/DFA_Compress.hpp"
using namespace std;

class DFAx_Product {
  const long long K;

 public:
  using state_t = long long;
  using symbol_t = int;
  DFAx_Product(long long K_) : K(K_) {}
  std::vector<symbol_t> alphabet() const { return {1, 2, 3, 4, 5, 6, 7, 8, 9}; }
  inline state_t initial_state() const { return 1; }
  inline state_t transition(state_t s, const symbol_t &a, int) const {
    return std::min(s * a, K + 1);
  }
  inline bool is_accept(state_t s) const { return s > K; }
  inline bool is_reject(state_t s) const { return false; }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  long long N, K;
  cin >> N >> K;
  int n = to_string(N).length();
  auto dfa_le = DFA_Inequality(N, 10);
  long long ans = dfa_dp<long long>(dfa_le, n) - 1;
  auto dfa_pro = DFA_Compress(DFAx_Product(K), n);
  for (int i = 1; i < n; i++) ans -= dfa_dp<long long>(dfa_pro, i);
  auto dfa_le2 =
      DFA_SymbolMap(dfa_le, dfa_pro.alphabet(), [](int x) { return x; });
  ans -= dfa_dp<long long>(dfa_le2 & dfa_pro, n);
  cout << ans << '\n';
  return 0;
}