#define PROBLEM "https://atcoder.jp/contests/abc154/tasks/abc154_e"
#include <bits/stdc++.h>
#include "src/Automaton/dfa_dp.hpp"
#include "src/Automaton/DFA_Inequality.hpp"
#include "src/Automaton/dfa_operations.hpp"
using namespace std;

class DFA_AEZ {
  const int K;

 public:
  using symbol_t = int;
  DFA_AEZ(int k) : K(k) {}
  std::vector<symbol_t> alphabet() const {
    return {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  }
  int initial_state() const { return 0; }
  int transition(int s, const symbol_t &a, int) const {
    return (s += (a != 0)) > K ? -1 : s;
  }
  bool is_accept(int s) const { return s == K; }
  inline int state_size() const { return K + 1; }
};
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  string N;
  int K;
  cin >> N >> K;
  cout << dfa_dp<int>(DFA_Inequality(N, 10) & DFA_AEZ(K), N.length()) << '\n';
  return 0;
}