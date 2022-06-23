#define PROBLEM "https://atcoder.jp/contests/abc154/tasks/abc154_e"
#include <bits/stdc++.h>
#include "src/Automaton/dfa_dp.hpp"
#include "src/Automaton/DFA_Le.hpp"
#include "src/Automaton/dfa_operations.hpp"
using namespace std;

struct DFA_AEZ {
  using state_t = int;
  using symbol_t = int;
  DFA_AEZ(int k) : K(k) {}
  std::set<symbol_t> alphabet() const {
    std::set<symbol_t> alp;
    for (int i = 10; i--;) alp.insert(i);
    return alp;
  }
  state_t initial_state() const { return 0; }
  state_t transition(const state_t s, const symbol_t &a) const {
    return std::min(s + (a != 0), K + 1);
  }
  bool is_accept(const state_t s) const { return s == K; }

 private:
  const int K;
};
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  string N;
  int K;
  cin >> N >> K;
  cout << dfa_dp<int>(DFA_Le(N, 10) & DFA_AEZ(K), N.length()) << '\n';
  return 0;
}