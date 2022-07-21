#define PROBLEM "https://atcoder.jp/contests/arc127/tasks/arc127_a"
#include <bits/stdc++.h>
#include "src/Automaton/dfa_dp.hpp"
#include "src/Automaton/dfa_operations.hpp"
#include "src/Automaton/DFA_Inequality.hpp"
using namespace std;

class DFA_Leading1s {
  const int n;

 public:
  using symbol_t = int;
  DFA_Leading1s(int n_) : n(n_), state_size(n + 1) {}
  inline std::vector<symbol_t> alphabet() const {
    return {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  }
  inline int initial_state() const { return 0; }
  inline int transition(int s, const symbol_t &a, int) const {
    if (s == n) return n;
    if (s == 0 && a == 0) return 0;
    return a == 1 ? s + 1 : -1;
  }
  inline bool is_accept(int s) const { return s == n; }
  const int state_size;
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  long long N;
  cin >> N;
  auto dfa_le = DFA_Inequality(N, 10, 16);
  long long ans = 0;
  for (int i = 1; i <= 16; i++)
    ans += dfa_dp<long long>(dfa_le & DFA_Leading1s(i), 16);
  cout << ans << '\n';
  return 0;
}