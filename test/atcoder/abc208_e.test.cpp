#define PROBLEM "https://atcoder.jp/contests/abc208/tasks/abc208_e"
#include <bits/stdc++.h>
#include "src/Automaton/dfa_dp.hpp"
#include "src/Automaton/DFA_Le.hpp"
#include "src/Automaton/dfa_operations.hpp"
using namespace std;

struct DFA_Product {
  using state_t = long long;
  using symbol_t = int;
  DFA_Product(long long K_) : K(K_) {}
  std::set<symbol_t> alphabet() const { return {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; }
  state_t initial_state() const { return 1; }
  state_t transition(const state_t s, const symbol_t &a) const {
    return std::min(s * a, K + 1);
  }
  bool is_accept(const state_t s) const { return s <= K; }

 private:
  const long long K;
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  long long N, K;
  cin >> N >> K;
  long long ans = 0, pw10 = 1;
  auto dfa_pro = DFA_Product(K);
  int n = to_string(N).length();
  for (int i = 1; i < n; i++, pw10 *= 10)
    ans += dfa_dp<long long>(dfa_pro, i) - pw10;
  ans += dfa_dp<long long>(DFA_Le(N, 10) & dfa_pro, n) - pw10;
  cout << ans << '\n';
  return 0;
}